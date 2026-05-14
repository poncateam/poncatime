#pragma once

/// \brief Run principal curvature estimation using Algebraic Shape operator
///
/// Points must contain values as [x y z nx ny nz ...]
/// Queries must contain values as [x y z ...]
///
int asoCurvatureEstimation(const double * points, int nPoints, const double * queries, int nQueries, double scale);


/// \brief Generate data for curvature estimation
void generate_data(double* point, int nPoints, double* queries, int nQueries, double dataScale);
