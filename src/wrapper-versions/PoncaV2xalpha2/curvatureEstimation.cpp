#include "curvatureEstimation.h"

#include <iostream>
#include "external/ponca/Ponca/Ponca"

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
Ponca::KdTreeDense<MyPointSimple> tree;

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

void generatePointClouds(Eigen::MatrixXd& points,
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
    // reset KdTree
    tree.build(std::vector<MyPointSimple>());
}

bool buildKdTree(const Eigen::MatrixXd& points)
{
    int nPoints  = points.rows();

    /// Bind dataset to Ponca representation
    std::vector<MyPointSimple> data;
    data.reserve(nPoints);
    for (int i = 0; i != nPoints; ++i)
    {
        data.emplace_back(points.row(i).head(3),points.row(i).tail(3));
    }
    tree.build(data);

    return nPoints != 0;
}

struct ComputeReturnType
{
    // number of fits
    int nbFit{0};
    // mean number of neighbors
    int kNeiMean{0};
};

template <typename Fit, bool range, typename Param>
ComputeReturnType computeFit(const Eigen::MatrixXd& queries, Param p)
{
    ComputeReturnType ret;

    if (tree.pointCount() == 0)
    {
        std::cerr<< "KdTree has not been initialized" << std::endl;
        return ret;
    }
    using NF     = typename Fit::NeighborFilter;
    using Point  = typename Fit::DataPoint;
    using Vector = typename Point::VectorType;
    using Scalar = typename Point::Scalar;

    int nQueries = queries.rows();

    // compute queries
    for (int i = 0; i != nQueries; ++i)
    {
        Vector q(queries.row(i).head(3));
        Fit f;
        f.setNeighborFilter(NF(q,Scalar(p)));
        // f.computeWithIds(tree.rangeNeighbors(q, scale), tree.points());
        if (range)
           f.computeWithIds(tree.rangeNeighbors(q, p), tree.points());
        else
           f.computeWithIds(tree.kNearestNeighbors(q, p), tree.points());
        if (f.isStable())
        {
            ret.nbFit++;
            ret.kNeiMean += f.getNumNeighbors();
        }
    }
    ret.kNeiMean /= ret.nbFit;

    return ret;
}

using NF        = DistWeightFunc<MyPointSimple, SmoothWeightKernel<double> > ;
using ASOBasket = Basket<MyPointSimple, NF, OrientedSphereFit>;
using ASOFit    = BasketDiff<ASOBasket, FitSpaceDer, OrientedSphereDer, MlsSphereFitDer>;
using PlaneFit  = Ponca::Basket<MyPointSimple, NF, CovariancePlaneFit>;

int asoCurvatureEstimation(const Eigen::MatrixXd& queries, double scale, int& meanNeiSize)
{
    auto ret = computeFit<ASOFit, true>(queries, scale);
    meanNeiSize = ret.kNeiMean;
    return ret.nbFit;
}

int planeFit(const Eigen::MatrixXd& queries, double scale, int& meanNeiSize)
{
    auto ret = computeFit<PlaneFit, true>(queries, scale);
    meanNeiSize = ret.kNeiMean;
    return ret.nbFit;
}
int asoCurvatureEstimation(const Eigen::MatrixXd& queries, int k)
{
    auto ret = computeFit<ASOFit, false>(queries, k);
    return ret.nbFit;
}

int planeFit(const Eigen::MatrixXd& queries, int k)
{
    auto ret = computeFit<PlaneFit, false>(queries, k);
    return ret.nbFit;
}

