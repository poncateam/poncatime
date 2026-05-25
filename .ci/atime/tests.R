remove.units <- "kilobytes"
base.ref <- "main"
test.list <- atime::atime_test_list(
  seconds.limit=0.1,
  result=TRUE,
  "kdTreeConstruction"=atime::atime_test(
    setup={
      N_points <- N*10
      dataScale <- 10
      N_list <- Poncatime:::generatePointClouds_interface(N_points, 0, dataScale)
      status <- Poncatime:::buildKdTree_interface(N_list$points)
    },
    expr=with(N_list, data.frame(status=Poncatime:::buildKdTree_interface(N_list$points)))
  ),
  "asoCurvatureEstimation(Np=Nq*10)"=atime::atime_test(
    setup={
      N_points <- N*10
      N_queries <- N
      dataScale <- 10
      N_list <- Poncatime:::generatePointClouds_interface(N_points, N_queries, dataScale)
      status <- Poncatime:::buildKdTree_interface(N_list$points)
    },
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
    expr=with(N_list, data.frame(count=Poncatime:::asoCurvatureEstimation_interface(N_list$queries, dataScale/5)))
  ),
  "planeFit(Np=Nq*10)"=atime::atime_test(
    setup={
      N_points <- N*10
      N_queries <- N
      dataScale <- 10
      N_list <- Poncatime:::generatePointClouds_interface(N_points, N_queries, dataScale)
      status <- Poncatime:::buildKdTree_interface(N_list$points)
    },
    expr=with(N_list, data.frame(count=Poncatime:::planeFit_interface(N_list$queries, dataScale/5)))
  ),
  "planeFit(Np=Nq)"=atime::atime_test(
    setup={
      N_points <- N
      N_queries <- N
      dataScale <- 10
      N_list <- Poncatime:::generatePointClouds_interface(N_points, N_queries, dataScale)
      status <- Poncatime:::buildKdTree_interface(N_list$points)
    },
    expr=with(N_list, data.frame(count=Poncatime:::planeFit_interface(N_list$queries, dataScale/5)))
  )
)
for(test.name in names(test.list)){
  test.list[[test.name]][["ponca v0.3"]] <- "PoncaV0x3"
  test.list[[test.name]][["ponca v1.0"]] <- "PoncaV1x0"
}
