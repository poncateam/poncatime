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

std::vector<int>logScale(int start, double base, int nbElements = 10)
{

    std::vector<int> scale;
    scale.reserve(nbElements);
    double current = start;

    // Generate the logarithmically spaced values
    std::generate_n(std::back_inserter(scale), nbElements, [&start, base, &current]() {
        int c (current);
        current *= base;
        return c;
    });
    return scale;
}

int main(int argc, char **argv)
{
    double dataScale  = 10;
    double scale  = dataScale / 10;

    Eigen::MatrixXd points;
    Eigen::MatrixXd queries;

    int start = 500;
    double base = 1.9;
    int nbSteps = 10;
    auto values = logScale(start, base, nbSteps);

    std::vector<std::string> names {
        "buildKdTree",
        "asoCurvatureEstimation",
        "planeFit"};
    std::vector<std::function<void(void)>> runs  {
        [&points](){buildKdTree(points);},
        [&queries, scale](){int k; asoCurvatureEstimation(queries, scale, k);},
        [&queries, scale](){int k; planeFit(queries, scale, k);}
    };

    json j;

    // prepare json structure
    {
        std::vector<double>placeholder;
        placeholder.resize(nbSteps);
        for (const auto& name : names)
        {
            j[name]["mean"] = placeholder;
            j[name]["var"] = placeholder;
        }
        j["steps"] = values;
    }

    int stepId = 0;
    for (auto v : values)
    {
        std::cout << "Run test with nb points = " << v << std::endl;
        int n = v; // number of points
        int q = v/10; // number of queries

        auto prepare = [&points, &queries, dataScale, n, q]()
        {
            points = Eigen::MatrixXd(n, 6);
            queries = Eigen::MatrixXd(q, 3);
            generatePointClouds(points, queries, dataScale);
            buildKdTree(points);
        };

        auto res = mesureTime<10>(prepare, runs, names);

        int index = 0;
        for (const auto& name : names)
        {
            j[name]["mean"][stepId] = res[index].mean;
            j[name]["var"][stepId] = res[index].var;
            ++index;
        }

        ++stepId;
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