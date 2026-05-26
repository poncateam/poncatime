library(testthat)

test_that("generate_data works", {
  N_points <- 10000
  N_queries <- 1000
  dataScale <- 10
  N_list <- Poncatime:::generatePointClouds_interface(N_points, N_queries, dataScale)
  expect_equal(nrow(N_list$queries), N_queries)
  expect_equal(nrow(N_list$points), N_points)
  status <- Poncatime:::buildKdTree_interface(N_list$points)
  expect_true(status)
  retValue <- with(N_list, Poncatime:::asoCurvatureEstimation_interface(N_list$queries, dataScale/5))
  expect_equal(retValue$nbFit, N_queries)
})
