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
[[nodiscard]] DataPoint getPointOnPlane(const typename DataPoint::VectorType& _vPosition,
                                       const typename DataPoint::Scalar& _width,
                                       const typename DataPoint::Scalar& _height,
                                       const typename DataPoint::VectorType& _localxAxis,
                                       const typename DataPoint::VectorType& _localyAxis,
                                       const bool _bAddPositionNoise = true)
{
    using Scalar     = typename DataPoint::Scalar;
    using VectorType = typename DataPoint::VectorType;

    const Scalar u = Eigen::internal::random<Scalar>(-_width / Scalar(2), _width / Scalar(2));
    const Scalar v = Eigen::internal::random<Scalar>(-_height / Scalar(2), _height / Scalar(2));

    VectorType vRandomPosition = _vPosition + u * _localxAxis + v * _localyAxis;

    if (_bAddPositionNoise)
    {
        vRandomPosition = vRandomPosition +
                          VectorType::Random().normalized() * Eigen::internal::random<Scalar>(0., 1. - MIN_NOISE);
    }

    return DataPoint(vRandomPosition, _localxAxis.cross(_localyAxis));
}

void generate_data(double* point, int nPoints, double* queries, int nQueries, double dataScale)
{
    MyPointSimple::VectorType position = MyPointSimple::VectorType::Random();

    for (int i = 0; i != nPoints; ++i)
    {
        auto p = getPointOnPlane<MyPointSimple>(position, dataScale,dataScale, {1,0,0}, {0,1,0});
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
        auto p = getPointOnPlane<MyPointSimple>(position, dataScale,dataScale, {1,0,0}, {0,1,0});
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
    // using Fit =  Basket<Point, W, CovariancePlaneFit>;

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