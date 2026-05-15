library(testthat)

test_that("generate_data works", {
  N_points <- 10000
  N_queries <- 1000
  dataScale <- 10
  N_list <- Poncatime:::generate_data_interface(N_points, N_queries, dataScale)
  expect_equal(nrow(N_list$queries), N_queries)
  expect_equal(nrow(N_list$points), N_points)
  (count <- with(N_list, Poncatime:::asoCurvatureEstimation_interface(points, queries, dataScale/5)))
  expect_equal(count, N_queries)
})
