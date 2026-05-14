#include <Rcpp.h>
#include "curvatureEstimation.h"

// [[Rcpp::export]]
//
Rcpp::IntegerVector asoCurvatureEstimationF
(const Rcpp::NumericVector pointVec,
 const Rcpp::NumericVector queriesVec,
 const numeric scale
 ){
  return asoCurvatureEstimationF(pointVec.begin(), pointVec.length(),
                                queriesVec.begin(), queriesVec.length(), scale);
}

