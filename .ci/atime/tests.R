remove.units <- "kilobytes"
base.ref <- "main"
test.list <- atime::atime_test_list(
  N=10^seq(2, 5, by=0.2),
  seconds.limit=0.1,
  result=TRUE,
  N=10^seq(3, 6, by=0.5),
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
    expr=with(N_list, Poncatime:::asoCurvatureEstimation_interface(N_list$queries, dataScale/5))
  ),
  "asoCurvatureEstimation(Np=Nq)"=atime::atime_test(
    setup={
      N_points <- N
      N_queries <- N
      dataScale <- 10
      N_list <- Poncatime:::generatePointClouds_interface(N_points, N_queries, dataScale)
      status <- Poncatime:::buildKdTree_interface(N_list$points)
    },
    expr=with(N_list, Poncatime:::asoCurvatureEstimation_interface(N_list$queries, dataScale/5))
  ),
  "planeFit(Np=Nq*10)"=atime::atime_test(
    setup={
      N_points <- N*10
      N_queries <- N
      dataScale <- 10
      N_list <- Poncatime:::generatePointClouds_interface(N_points, N_queries, dataScale)
      status <- Poncatime:::buildKdTree_interface(N_list$points)
    },
    expr=with(N_list, Poncatime:::planeFit_interface(N_list$queries, dataScale/5))
  ),
  "planeFit(Np=Nq)"=atime::atime_test(
    setup={
      N_points <- N
      N_queries <- N
      dataScale <- 10
      N_list <- Poncatime:::generatePointClouds_interface(N_points, N_queries, dataScale)
      status <- Poncatime:::buildKdTree_interface(N_list$points)
    },
    expr=with(N_list, Poncatime:::planeFit_interface(N_list$queries, dataScale/5))
  )
)
for(test.name in names(test.list)){
  # test.list[[test.name]][["ponca v0.3"]] <- "PoncaV0x3"
  test.list[[test.name]][["ponca v1.0"]] <- "PoncaV1x0"
  # test.list[[test.name]][["ponca v1.2"]] <- "PoncaV1x2"
  # test.list[[test.name]][["ponca v1.3"]] <- "PoncaV1x3"
  test.list[[test.name]][["ponca v1.4"]] <- "PoncaV1x4"
  test.list[[test.name]][["ponca v2.alpha0"]] <- "PoncaV2xalpha0"
}
