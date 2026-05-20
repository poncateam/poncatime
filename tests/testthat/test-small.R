N_data <- 100
N_queries <- 10
dataScale <- 20
N_list <- Poncatime:::generatePointClouds_interface(N_data, N_queries, dataScale)
status <- Poncatime:::buildKdTree_interface(N_list$points)
count <- with(N_list, Poncatime:::asoCurvatureEstimation_interface(N_list$queries, dataScale))
