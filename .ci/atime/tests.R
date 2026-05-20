remove.units <- "kilobytes"
base.ref <- "main"
test.list <- atime::atime_test_list(
  seconds.limit=0.1,
  "asoCurvatureEstimation(Np=Nq*10)"=atime::atime_test(
    setup={
      N_points <- N*10
      N_queries <- N
      dataScale <- 10
      N_list <- Poncatime:::generatePointClouds_interface(N_points, N_queries, dataScale)
      status <- Poncatime:::buildKdTree_interface(N_list$points)
    },
    result=TRUE,
    first_valgrind_ok="c9c2aa49c23f7b06ab06fa67e5f4066856c801f4", # first version which compiles and runs without valgrind read errors.
    expr=with(N_list, data.frame(count=Poncatime:::asoCurvatureEstimation_interface(N_list$queries, dataScale/5)))
  ),
  "asoCurvatureEstimation(Np=Nq)"=atime::atime_test(
    setup={
      N_points <- N
      N_queries <- N
      dataScale <- 10
      N_list <- Poncatime:::generatePointClouds_interface(N_points, N_queries, dataScale)
      status <- Poncatime:::buildKdTree_interface(N_list$points)
    },
    result=TRUE,
    first_valgrind_ok="c9c2aa49c23f7b06ab06fa67e5f4066856c801f4", # first version which compiles and runs without valgrind read errors.
    expr=with(N_list, data.frame(count=Poncatime:::asoCurvatureEstimation_interface(N_list$queries, dataScale/5)))
  )
)
