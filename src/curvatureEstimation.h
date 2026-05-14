#pragma once

#include "export.h"

/// \brief Run principal curvature estimation using Algebraic Shape operator
///
/// Points must contain values as [x y z nx ny nz ...]
/// Queries must contain values as [x y z ...]
///
extern "C" EXPORT int asoCurvatureEstimation(const double * points, int nPoints, const double * queries, int nQueries, double scale);


