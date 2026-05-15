N_data <- 10000
N_queries <- 1000
N_list <- Poncatime:::generate_data_interface(N_data, N_queries, 10)
count <- with(N_list, Poncatime:::asoCurvatureEstimation_interface(points, queries, 10))
