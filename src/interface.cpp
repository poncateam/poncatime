#include <Rcpp.h>
#include "curvatureEstimation.h"

// [[Rcpp::export]]
int asoCurvatureEstimation_interface
(const Rcpp::NumericVector pointVec,
 const Rcpp::NumericVector queriesVec,
 const double scale
 ){
  return asoCurvatureEstimation(pointVec.begin(), pointVec.length(),
                                queriesVec.begin(), queriesVec.length(), scale);
}

// [[Rcpp::export]]
Rcpp::List generate_data_interface
(const int N_data, const int N_queries, double dataScale){
  Rcpp::NumericVector pointVec(N_data*6);
  Rcpp::NumericVector queriesVec(N_queries*3);
  generate_data(pointVec.begin(), N_data, queriesVec.begin(), N_queries, dataScale);
  return Rcpp::List::create
    (Rcpp::Named("pointVec", pointVec),
     Rcpp::Named("queriesVec", queriesVec));
}
