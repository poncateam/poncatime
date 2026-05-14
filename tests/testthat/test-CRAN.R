library(testthat)

test_that("generate_data works", {
  N_data <- 10000
  N_queries <- 100
  dataScale <- 10
  N_list <- Poncatime:::generate_data_interface(N_data, N_queries, dataScale)
  expect_equal(length(N_list$pointVec), 6*N_data)
  expect_equal(length(N_list$queriesVec), 3*N_queries)
  (count <- with(N_list, Poncatime:::asoCurvatureEstimation_interface(pointVec, queriesVec, dataScale/10)))
})



