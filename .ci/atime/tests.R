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
    ponca_v0_3="95a7450b81344da32e86deebb004c4d8b790ba2c", # ponca v0.3 with Atime wrapper with KdTree API
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
    ponca_v0_3="c9c2aa49c23f7b06ab06fa67e5f4066856c801f4", # ponca v0.3 with Atime wrapper with KdTree API
    expr=with(N_list, data.frame(count=Poncatime:::asoCurvatureEstimation_interface(N_list$queries, dataScale/5)))
  )
)
