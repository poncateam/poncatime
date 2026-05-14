#include "curvatureEstimation.h"

#include <iostream>

int main(int argc, char **argv)
{
    int nbPoints  = 10000;
    int nbQueries = 100;
    double dataScale  = 10;
    double scale  = dataScale / 5;
    double* points  = new double[6*nbPoints];
    double* queries = new double[3*nbQueries];

    generate_data(points, nbPoints, queries, nbQueries, dataScale);

    // std::cout << "#Data" << std::endl;
    // std::cout << "# x y z nx ny nz" << std::endl;
    // for (int i = 0; i < nbPoints; ++i)
    // {
    //     std::cout << points[6*i] << " "
    //               << points[6*i+1] << " "
    //               << points[6*i+2] << " "
    //               << points[6*i+3] << " "
    //               << points[6*i+4] << " "
    //               << points[6*i+5] << std::endl;
    // }
    //
    // std::cout << "#Queries" << std::endl;
    // std::cout << "# x y z" << std::endl;
    // for (int i = 0; i < nbQueries; ++i)
    // {
    //     std::cout << queries[3*i] << " "
    //               << queries[3*i+1] << " "
    //               << queries[3*i+2] << std::endl;
    // }


    int ret = asoCurvatureEstimation(points, nbPoints, queries, nbQueries, scale);

    delete [] points;
    delete [] queries;

    std::cout << "Number of fits: " << ret << " (over " << nbQueries << " tries)" << std::endl;
    /// \brief Generate data for curvature estimation
}