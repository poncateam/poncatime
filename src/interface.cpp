#include <Rcpp.h>
#include "curvatureEstimation.h"

// [[Rcpp::export]]
Rcpp::IntegerVector asoCurvatureEstimation_interface
(const Rcpp::NumericVector pointVec,
 const Rcpp::NumericVector queriesVec,
 const double scale
 ){
  return asoCurvatureEstimation(pointVec.begin(), pointVec.length(),
                                queriesVec.begin(), queriesVec.length(), scale);
}

