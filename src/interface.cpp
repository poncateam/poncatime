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
Rcpp::List generate_data_interface
(const int N_data,
 const int N_queries,
 const double dataScale){
  Eigen::MatrixXd points(N_data, 6), queries(N_queries, 3);
  generate_data(points, queries, dataScale);
  return Rcpp::List::create
    (Rcpp::Named("points", points),
     Rcpp::Named("queries", queries));
}
