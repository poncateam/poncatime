#include <Rcpp.h>
#include <RcppEigen.h>
#include "curvatureEstimation.h"

// [[Rcpp::depends(RcppEigen)]]

// [[Rcpp::export]]
int asoCurvatureEstimation_interface
(const Eigen::MatrixXd& points,
 const Eigen::MatrixXd& queries,
 const double scale
 ){
  return asoCurvatureEstimation(points, queries, scale);
}

// [[Rcpp::export]]
void generate_data_interface
(const Eigen::MatrixXd& points,
 const Eigen::MatrixXd& queries,
 double dataScale){
  generate_data(pointVec.begin(), N_data, queriesVec.begin(), N_queries, dataScale);
}
