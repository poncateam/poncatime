#include <RcppEigen.h>
#include "curvatureEstimation.h"

// [[Rcpp::depends(RcppEigen)]]

// [[Rcpp::export]]
Rcpp::List generatePointClouds_interface
(const int N_data,
 const int N_queries,
 const double dataScale){
  Eigen::MatrixXd points(N_data, 6), queries(N_queries, 3);
  generatePointClouds(points, queries, dataScale);
  return Rcpp::List::create
    (Rcpp::Named("points", points),
     Rcpp::Named("queries", queries));
}

// [[Rcpp::export]]
bool buildKdTree_interface
(const Eigen::MatrixXd& points){
 return buildKdTree(points);
}

// [[Rcpp::export]]
int asoCurvatureEstimation_interface
(const Eigen::MatrixXd& queries,
 const double scale
 ){
 return asoCurvatureEstimation(queries, scale);
}

// [[Rcpp::export]]
int planeFit_interface
(const Eigen::MatrixXd& queries,
 const double scale
 ){
 return planeFit(queries, scale);
}
