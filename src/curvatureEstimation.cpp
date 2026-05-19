#include "curvatureEstimation.h"

#include "external/ponca/Ponca/Ponca"
//#include <Ponca/Ponca>

#define DIMENSION 3

using namespace Ponca;

template<typename InMType>
class MyPointMap
{
public:
    enum {Dim = DIMENSION};
    using Scalar = double;
    typedef Eigen::Matrix<Scalar, Dim, 1>   VectorType;
    typedef Eigen::Matrix<Scalar, Dim, Dim> MatrixType;
    typedef Eigen::VectorBlock<InMType> InnerVectorType;

    PONCA_MULTIARCH inline MyPointMap(InMType &mat, int _pId)
        : m_pos  (mat.col(_pId).head(3)),
        m_normal (mat.col(_pId).tail(3))
    {}

    PONCA_MULTIARCH inline const InnerVectorType& pos()    const { return m_pos; }
    PONCA_MULTIARCH inline const InnerVectorType& normal() const { return m_normal; }

public:
    InnerVectorType m_pos, m_normal;
    // VectorType m_pos, m_normal;
};

// struct PassThroughConverter{
//     inline void operator()( const std::vector< MyPointMap > &&i, std::vector< MyPointMap > & o ) {
//         o = std::move(i);
//     }
// };

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



/// Generate acceleration structure
Ponca::KdTree<MyPointSimple> tree;

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

void generate_data(Eigen::MatrixXd& points,
                   Eigen::MatrixXd& queries,
                   double dataScale)
{
    MyPointSimple::VectorType position = MyPointSimple::VectorType::Random();

    for (int i = 0; i != points.rows(); ++i)
    {
        auto p = getPointOnPlane<MyPointSimple>(position, dataScale,dataScale, {1,0,0}, {0,1,0});
        points.row(i) << p.pos().x(), p.pos().y(),p.pos().z(),p.normal().x(),p.normal().y(),p.normal().z();
    }

    for (int i = 0; i != queries.rows(); ++i)
    {
        auto p = getPointOnPlane<MyPointSimple>(position, dataScale,dataScale, {1,0,0}, {0,1,0});
        queries.row(i) << p.pos().x(), p.pos().y(),p.pos().z();
    }

    // using Fit =  Basket<Point, W, CovariancePlaneFit>;

    int nPoints  = points.rows();
    int nQueries = queries.rows();

    /// Bind dataset to Ponca representation
    std::vector<MyPointSimple> data;
    data.reserve(nPoints);
    for (int i = 0; i != nPoints; ++i)
    {
        data.emplace_back(points.row(i).head(3),points.row(i).tail(3));
    }
    tree.build(data); //, PassThroughConverter<Scalar>());
}


int asoCurvatureEstimation(const Eigen::MatrixXd& points,
                           const Eigen::MatrixXd& queries,
                           double scale)
{
    using Point     = MyPointSimple;
    using Vector    = Point::VectorType;
    using VectorMap = Eigen::Map<const Vector>;

    using W   = DistWeightFunc<Point, SmoothWeightKernel<double> > ;
    using Fit =  Basket<Point, W, OrientedSphereFit, OrientedSphereSpaceDer, MlsSphereFitDer>;

    int nPoints  = points.rows();
    int nQueries = queries.rows();

    // compute queries
    int ret = 0;
    for (int i = 0; i != nQueries; ++i)
    {
        Vector q(queries.row(i).head(3));
        Fit f;
        f.setWeightFunc(W(scale));
        f.init(q);
        f.computeWithIds(tree.range_neighbors(q, scale), tree.point_data());
        if (f.isStable()) ret++;
    }

    return ret;
}