#include "curvatureEstimation.h"

#include "external/ponca/Ponca/Ponca"
//#include <Ponca/Ponca>

#define DIMENSION 3

using namespace Ponca;

class MyPoint
{
public:
    enum {Dim = DIMENSION};
    using Scalar = double;
    typedef Eigen::Matrix<Scalar, Dim, 1>   VectorType;
    typedef Eigen::Matrix<Scalar, Dim, Dim> MatrixType;

    PONCA_MULTIARCH inline MyPoint(const Scalar* _interlacedArray, int _pId)
        : m_pos   (Eigen::Map< const VectorType >(_interlacedArray + Dim*2*_pId  )),
        m_normal(Eigen::Map< const VectorType >(_interlacedArray + Dim*2*_pId+Dim))
    {}

    // PONCA_MULTIARCH inline const Eigen::Map< const VectorType >& pos()    const { return m_pos; }
    // PONCA_MULTIARCH inline const Eigen::Map< const VectorType >& normal() const { return m_normal; }
    PONCA_MULTIARCH inline const VectorType& pos()    const { return m_pos; }
    PONCA_MULTIARCH inline const VectorType& normal() const { return m_normal; }

public:
    // Eigen::Map< const VectorType > m_pos, m_normal;
    VectorType m_pos, m_normal;
};

struct PassThroughConverter{
    inline void operator()( const std::vector< MyPoint > &&i, std::vector< MyPoint > & o ) {
        o = std::move(i);
    }
};

class MyPointSimple
{
public:
    enum {Dim = DIMENSION};
    using Scalar = double;
    typedef Eigen::Matrix<Scalar, Dim, 1>   VectorType;
    typedef Eigen::Matrix<Scalar, Dim, Dim> MatrixType;

    PONCA_MULTIARCH inline MyPointSimple(const VectorType& p, const VectorType& n)
        : m_pos   (p), m_normal(n)
    {}
    PONCA_MULTIARCH inline const VectorType& pos()    const { return m_pos; }
    PONCA_MULTIARCH inline const VectorType& normal() const { return m_normal; }

private:
    VectorType m_pos, m_normal;
};

#define MIN_NOISE 0.99
#define MAX_NOISE 1.01
/*! \brief Generate points on a plane */
template <typename DataPoint>
[[nodiscard]] DataPoint getPointOnPlane(const typename DataPoint::VectorType _vPosition,
                                        const typename DataPoint::VectorType _vNormal,
                                        const typename DataPoint::Scalar _radius,
                                        const bool _bAddPositionNoise = true, const bool _bAddNormalNoise = true,
                                        const bool _bReverseNormals = false)
{
    using Scalar         = typename DataPoint::Scalar;
    using VectorType     = typename DataPoint::VectorType;
    using QuaternionType = Eigen::Quaternion<Scalar>;

    VectorType vRandom;
    VectorType vRandomDirection = VectorType::Zero();
    VectorType vRandomPoint     = VectorType::Zero();
    VectorType vLocalUp         = _vNormal;

    do
    {
        vRandom          = VectorType::Random().normalized(); // Direction in the unit sphere
        vRandomDirection = vRandom.cross(vLocalUp);
    } while (vRandomDirection == VectorType::Zero());

    vRandomDirection = vRandomDirection.normalized();
    vRandomPoint     = vRandomDirection * _radius;
    vRandomPoint += _vPosition;

    if (_bAddPositionNoise)
    {
        vRandomPoint = vRandomPoint + VectorType::Random().normalized() *
                                          Eigen::internal::random<Scalar>(Scalar(0), Scalar(1. - MIN_NOISE));
    }

    if (_bAddNormalNoise)
    {
        VectorType vLocalLeft  = vLocalUp.cross(vRandomDirection);
        VectorType vLocalFront = vLocalLeft.cross(vLocalUp);

        Scalar rotationAngle     = Eigen::internal::random<Scalar>(Scalar(-M_PI / 16.), Scalar(M_PI / 16.));
        VectorType vRotationAxis = vLocalLeft;
        QuaternionType qRotation = QuaternionType(Eigen::AngleAxis<Scalar>(rotationAngle, vRotationAxis));
        qRotation                = qRotation.normalized();
        vLocalUp                 = qRotation * vLocalUp;

        rotationAngle = Eigen::internal::random<Scalar>(Scalar(-M_PI / 16.), Scalar(M_PI / 16.));
        vRotationAxis = vLocalFront;
        qRotation     = QuaternionType(Eigen::AngleAxis<Scalar>(rotationAngle, vRotationAxis));
        qRotation     = qRotation.normalized();
        vLocalUp      = qRotation * vLocalUp;
    }

    if (_bReverseNormals)
    {
        const float reverse = Eigen::internal::random<float>(0.f, 1.f);
        if (reverse > 0.5f)
            vLocalUp = -vLocalUp;
    }

    return DataPoint(vRandomPoint, vLocalUp);
}

void generate_data(double* point, int nPoints, double* queries, int nQueries, double dataScale)
{
    MyPointSimple::VectorType position = MyPointSimple::VectorType::Random();
    MyPointSimple::VectorType normal = MyPointSimple::VectorType::Random();
    for (int i = 0; i != nPoints; ++i)
    {
        auto p = getPointOnPlane<MyPointSimple>(position, normal, dataScale);
        double* pp = point + 2*DIMENSION*i;
        pp[0] = p.pos().x();
        pp[1] = p.pos().y();
        pp[2] = p.pos().z();
        pp[3] = p.normal().x();
        pp[4] = p.normal().y();
        pp[5] = p.normal().z();
    }

    for (int i = 0; i != nQueries; ++i)
    {
        auto p = getPointOnPlane<MyPointSimple>(position, normal, dataScale);
        double* pp = queries + DIMENSION*i;
        pp[0] = p.pos().x();
        pp[1] = p.pos().y();
        pp[2] = p.pos().z();
    }
}


int asoCurvatureEstimation(const double * points, int nPoints, const double *queries, int nQueries, double scale)
{
    using Point     = MyPoint;
    using Vector    = Point::VectorType;
    using VectorMap = Eigen::Map<const Vector>;

    using W   = DistWeightFunc<Point, SmoothWeightKernel<double> > ;
    using Fit =  Basket<Point, W, OrientedSphereFit, OrientedSphereSpaceDer, MlsSphereFitDer>;

    /// Bind dataset to Ponca representation
    std::vector<Point> data;
    data.reserve(nPoints);
    for (int i = 0; i != nPoints; ++i)
    {
        data.push_back(Point(points, i));
    }

    /// Generate acceleration structure
    Ponca::KdTree<Point> tree;
    tree.build(data); //, PassThroughConverter<Scalar>());

    // compute queries
    int ret = 0;
    for (int i = 0; i != nQueries; ++i)
    {
        VectorMap q(queries + DIMENSION*nQueries);
        Fit f;
        f.setWeightFunc(W(scale));
        f.init(q);
        f.computeWithIds(tree.range_neighbors(q, scale), data);
        if (f.isStable()) ret++;
    }

    return ret;
}