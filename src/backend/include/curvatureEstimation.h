#pragma once

#include "export.h"

/// \brief Run principal curvature estimation using Algebraic Shape operator
///
/// Points must contain values as [x y z nx ny nz ...]
/// Queries must contain values as [x y z ...]
///
extern "C" EXPORT int asoCurvatureEstimationF(const float * points, int nPoints, const float * queries, int nQueries, float scale);
