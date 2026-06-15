#pragma once

#include <Eigen/Dense>


/// \brief Default function use to generate the point cloud and the queries for the tests
void generatePointClouds(Eigen::MatrixXd& points,
                         Eigen::MatrixXd& queries,
                         double dataScale);

/// \brief Compute the KdTree from an input set of points.
///
/// \note The KdTree is made accessible to the estimation functions through a global variable.
///
/// \note This function can be used either to measure the performances of the KdTree construction,
///       or to prepare the KdTree for the estimation functions below
///
/// \return false if points is empty
bool buildKdTree(const Eigen::MatrixXd& points);


/*********************************************************************************************/
/*       Estimation functions                                                                */
/* (require a preliminary call to generateData and then buildKdTree)                         */
/* Several estimation functions can be called sequentially without recomputing the KdTree    */
/*********************************************************************************************/
/// \brief Run principal curvature estimation using Algebraic Shape operator on range queries
/// \warning requires a Kdtree, \see buildKdTree
///
/// \param meanNeiSize mean number of points in the neighborhood
/// \return the number of points correctly analyzed, or -1 if the KdTree has not been initialized first
int asoCurvatureEstimation(const Eigen::MatrixXd& queries, double scale, int &meanNeiSize);
/// \brief Run covariance plane fitting on range queries
/// \warning requires a Kdtree, \see buildKdTree
///
/// \param meanNeiSize mean number of points in the neighborhood
/// \return the number of points correctly analyzed, or -1 if the KdTree has not been initialized first
int planeFit(const Eigen::MatrixXd& queries, double scale, int &meanNeiSize);
/// \brief Run principal curvature estimation using Algebraic Shape operator  on k-neighbors
/// \warning requires a Kdtree, \see buildKdTree
///
/// \return the number of points correctly analyzed, or -1 if the KdTree has not been initialized first
int asoCurvatureEstimation(const Eigen::MatrixXd& queries, int k);
/// \brief Run covariance plane fitting on k-neighbors
/// \warning requires a Kdtree, \see buildKdTree
///
/// \return the number of points correctly analyzed, or -1 if the KdTree has not been initialized first
int planeFit(const Eigen::MatrixXd& queries, int k);