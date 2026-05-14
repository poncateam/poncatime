library(testthat)

test_that("generate_data works", {
  N_list <- Poncatime:::generate_data_interface(10, 100, 10)
  expect_equal(length(N_list$pointVec), 60)
  expect_equal(length(N_list$queriesVec), 300)
  count <- with(N_list, Poncatime:::asoCurvatureEstimation_interface(pointVec, queriesVec, 10))
})



