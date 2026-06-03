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


template<int nbRuns, typename PProcess, typename RProcessList>
std::vector<TimeResult> mesureTime(PProcess prepare, RProcessList runList, const std::vector<std::string>& names)
{
    std::vector<TimeResult> res;
    res.resize(names.size());

    // collect measurements
    std::vector<std::array<int, nbRuns>> times;
    times.resize(names.size());

    for (int i = 0; i != nbRuns; ++i)
    {
        prepare();
        int j = 0;
        for (auto run : runList)
        {
            auto start = std::chrono::steady_clock::now();
            run();
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;
            times[j][i] = elapsed.count();
            res[j].mean += times[j][i];
            ++j;
        }
    }

    int j = 0;
    for (const auto& name : names)
    {
        res[j].mean /= float(nbRuns);

        // compute mean v
        for (int i = 0; i != nbRuns; ++i)
        {
            res[j].var += std::pow((times[j][i]-res[j].mean),2);
        }
        res[j].var /= float(nbRuns);
        ++j;
    }

    return res;
}

int main(int argc, char **argv)
{
    int nbPoints  = 10000;
    int nbQueries = 10000;
    double dataScale  = 10;
    double scale  = dataScale / 10;

    Eigen::MatrixXd points;
    Eigen::MatrixXd queries;

    int n = nbPoints;
    int q = nbQueries;
    auto prepare = [&points, &queries, dataScale, n, q]()
    {
        points = Eigen::MatrixXd(n, 6);
        queries = Eigen::MatrixXd(q, 3);
        generatePointClouds(points, queries, dataScale);
        buildKdTree(points);
    };

    std::vector<std::string> names {
        "buildKdTree",
        "asoCurvatureEstimation",
        "planeFit"};
    std::vector<std::function<void(void)>> runs  {
        [&points](){buildKdTree(points);},
        [&queries, scale](){int k; asoCurvatureEstimation(queries, scale, k);},
        [&queries, scale](){int k; planeFit(queries, scale, k);}
    };
    auto res = mesureTime<10>(prepare, runs, names);

    // transform output as json
    json j;
    int index = 0;
    for (const auto& n : names)
    {
        j[n]["mean"] = res[index].mean;
        j[n]["var"] = res[index].var;
        ++index;
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