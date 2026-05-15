#pragma once

#include <Eigen/Dense>

/// \brief Run principal curvature estimation using Algebraic Shape operator
///
///
int asoCurvatureEstimation(const Eigen::MatrixXd& points,
                           const Eigen::MatrixXd& queries,
                           double scale);


/// \brief Generate data for curvature estimation
void generate_data(Eigen::MatrixXd& points,
                   Eigen::MatrixXd& queries,
                   double dataScale);
