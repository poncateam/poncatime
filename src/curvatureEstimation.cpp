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