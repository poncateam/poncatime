test.list <- atime::atime_test_list(
  asoCurvatureEstimation=atime::atime_test(
    setup={
      N_points <- N*10
      N_queries <- N
      dataScale <- 10
      N_list <- Poncatime:::generate_data_interface(N_points, N_queries, dataScale)
    },
    result=TRUE,
    expr=with(N_list, data.frame(count=Poncatime:::asoCurvatureEstimation_interface(points, queries, dataScale/5)))
  )
)
