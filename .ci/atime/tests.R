remove.units <- "kilobytes"
base.ref <- "main"
version.colors = c(
  HEAD="#6EA358", 
  "ponca v1.0" = "#FFD1C7",
  "ponca v1.2" = "#FFB5A3",
  "ponca v1.3" = "#FF9C85",
  "ponca v1.4" = "#FF7D5E",
  "ponca v2.alpha0" = "#C9DCFF",
  "ponca v2.alpha1" = "#A1C1FF",
  "ponca v2.alpha2" = "#82ABFF",
  "ponca v2.alpha3" = "#548CFF"
)

test.list <- atime::atime_test_list(
  seconds.limit=0.1,
  result=TRUE,
  N=10^seq(2, 6, by=0.5),
  "kdTreeConstruction"=atime::atime_test(
    setup={
      N_points <- N*10
      dataScale <- 10
      N_list <- Poncatime:::generatePointClouds_interface(N_points, 0, dataScale)
      status <- Poncatime:::buildKdTree_interface(N_list$points)
    },
    expr=data.frame(nbFit=Poncatime:::buildKdTree_interface(N_list$points), meanNeiSize=0)
  ),
  "asoCurvatureEstimation(Np=Nq*10)"=atime::atime_test(
    setup={
      N_points <- N*10
      N_queries <- N
      dataScale <- 10
      N_list <- Poncatime:::generatePointClouds_interface(N_points, N_queries, dataScale)
      status <- Poncatime:::buildKdTree_interface(N_list$points)
    },
    expr=Poncatime:::asoCurvatureEstimation_interface(N_list$queries, dataScale/5)
  ),
  "asoCurvatureEstimation(Np=Nq)"=atime::atime_test(
    setup={
      N_points <- N
      N_queries <- N
      dataScale <- 10
      N_list <- Poncatime:::generatePointClouds_interface(N_points, N_queries, dataScale)
      status <- Poncatime:::buildKdTree_interface(N_list$points)
    },
    expr=Poncatime:::asoCurvatureEstimation_interface(N_list$queries, dataScale/5)
  ),
  "planeFit(Np=Nq*10)"=atime::atime_test(
    setup={
      N_points <- N*10
      N_queries <- N
      dataScale <- 10
      N_list <- Poncatime:::generatePointClouds_interface(N_points, N_queries, dataScale)
      status <- Poncatime:::buildKdTree_interface(N_list$points)
    },
    expr=Poncatime:::planeFit_interface(N_list$queries, dataScale/5)
  ),
  "planeFit(Np=Nq)"=atime::atime_test(
    setup={
      N_points <- N
      N_queries <- N
      dataScale <- 10
      N_list <- Poncatime:::generatePointClouds_interface(N_points, N_queries, dataScale)
      status <- Poncatime:::buildKdTree_interface(N_list$points)
    },
    expr=Poncatime:::planeFit_interface(N_list$queries, dataScale/5)
  )
)
for(test.name in names(test.list)){
  # test.list[[test.name]][["ponca v0.3"]] <- "PoncaV0x3"
  test.list[[test.name]][["ponca v1.0"]] <- "PoncaV1x0"
  # test.list[[test.name]][["ponca v1.2"]] <- "PoncaV1x2"
  # test.list[[test.name]][["ponca v1.3"]] <- "PoncaV1x3"
  test.list[[test.name]][["ponca v1.4"]] <- "PoncaV1x4"
  test.list[[test.name]][["ponca v2.alpha0"]] <- "PoncaV2xalpha0"
  test.list[[test.name]][["ponca v2.alpha1"]] <- "PoncaV2xalpha1"
  test.list[[test.name]][["ponca v2.alpha2"]] <- "PoncaV2xalpha2"
}

