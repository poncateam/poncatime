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
Rcpp::DataFrame  asoCurvatureEstimation_interface
(const Eigen::MatrixXd& queries,
 const double scale
 ){
 int meanNeiSize;
 int nbFit = asoCurvatureEstimation(queries, scale, meanNeiSize);
 return Rcpp::DataFrame::create
   (Rcpp::Named("nbFit", nbFit),
    Rcpp::Named("meanNeiSize", meanNeiSize));
}

// [[Rcpp::export]]
Rcpp::DataFrame  planeFit_interface
(const Eigen::MatrixXd& queries,
 const double scale
 ){
 int meanNeiSize;
 int nbFit = planeFit(queries, scale, meanNeiSize);
 return Rcpp::DataFrame::create
   (Rcpp::Named("nbFit", nbFit),
    Rcpp::Named("meanNeiSize", meanNeiSize));
}

// [[Rcpp::export]]
Rcpp::DataFrame  asoCurvatureEstimationK_interface
(const Eigen::MatrixXd& queries,
 const int k
 ){
 int meanNeiSize = k;
 int nbFit = asoCurvatureEstimation(queries, k);
 return Rcpp::DataFrame::create
   (Rcpp::Named("nbFit", nbFit),
    Rcpp::Named("meanNeiSize", meanNeiSize));
}

// [[Rcpp::export]]
Rcpp::DataFrame  planeFitK_interface
(const Eigen::MatrixXd& queries,
 const int k
 ){
 int meanNeiSize = k;
 int nbFit = planeFit(queries, k);
 return Rcpp::DataFrame::create
   (Rcpp::Named("nbFit", nbFit),
    Rcpp::Named("meanNeiSize", meanNeiSize));
}

