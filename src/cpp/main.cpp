#include "curvatureEstimation.h"

#include <iostream>

int main(int argc, char **argv)
{
    int nbPoints  = 10000;
    int nbQueries = 100;
    double dataScale  = 10;
    double scale  = dataScale / 5;

    Eigen::MatrixXd points(nbPoints, 6);
    Eigen::MatrixXd queries(nbQueries, 3);

    generatePointClouds(points, queries, dataScale);
    if( !buildKdTree(points) )
    {
        return EXIT_FAILURE;
    }

    int ret = asoCurvatureEstimation(queries, scale);
    std::cout << "[ASO] Number of fits: " << ret << " (over " << nbQueries << " tries)" << std::endl;
    ret = planeFit(queries, scale);
    std::cout << "[PLANE] Number of fits: " << ret << " (over " << nbQueries << " tries)" << std::endl;

    return EXIT_SUCCESS;
}