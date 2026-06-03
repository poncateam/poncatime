#include "curvatureEstimation.h"

#include <fstream>
#include "nlohmann/json.hpp"

#include <iostream>

using json = nlohmann::json;

struct TimeResult
{
    float mean{0}; /// Mean time in msec
    float var{0};  /// Variance in msec
};

template<int nbRuns = 10, typename Process>
TimeResult mesureTime(Process p)
{
    TimeResult res;
    // collect measurements
    std::array<int, nbRuns> times;
    res.mean = 0;
    for (int i = 0; i != nbRuns; ++i)
    {
        auto start = std::chrono::steady_clock::now();
        p();
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        res.mean += times[i] = elapsed.count();
    }
    res.mean /= float(nbRuns);

    // compute mean v
    for (int i = 0; i != nbRuns; ++i)
    {
        res.var += std::pow((times[i]-res.mean),2);
    }
    res.var /= float(nbRuns);

    return res;
}

int main(int argc, char **argv)
{
    int nbPoints  = 100000;
    int nbQueries = 1000;
    double dataScale  = 10;
    double scale  = dataScale / 5;

    Eigen::MatrixXd points(nbPoints, 6);
    Eigen::MatrixXd queries(nbQueries, 3);

    generatePointClouds(points, queries, dataScale);
    if( !buildKdTree(points) )
    {
        return EXIT_FAILURE;
    }

    json j;

    {
        auto res = mesureTime([&points](){buildKdTree(points);});
        j["buildKdTree"]["mean"] = res.mean;
        j["buildKdTree"]["var"] = res.var;
    }

    {
        int meanK;
        auto res = mesureTime([&queries, scale, &meanK]()
        {
            asoCurvatureEstimation(queries, scale, meanK);
        });
        j["asoCurvatureEstimation"]["mean"] = res.mean;
        j["asoCurvatureEstimation"]["var"] = res.var;
        j["asoCurvatureEstimation"]["meanK"] = meanK;
    }

    {
        int meanK;
        auto res = mesureTime([&queries, scale, &meanK]()
        {
            planeFit(queries, scale, meanK);
        });
        j["planeFit"]["mean"] = res.mean;
        j["planeFit"]["var"] = res.var;
        j["planeFit"]["meanK"] = meanK;
    }

    // write prettified JSON
    std::ofstream o("run_output.json");
    o << std::setw(4) << j<< std::endl;
    o.close();



    // int ret = asoCurvatureEstimation(queries, scale, meanK);
    // std::cout << "[ASO] Number of fits: " << ret << " (over " << nbQueries << " tries) with " << meanK << " neighbors in average" << std::endl;
    // ret = planeFit(queries, scale, meanK);
    // std::cout << "[PLANE] Number of fits: " << ret << " (over " << nbQueries << " tries) with " << meanK << " neighbors in average" << std::endl;

    return EXIT_SUCCESS;
}