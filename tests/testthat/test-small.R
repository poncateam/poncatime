N_data <- 100
N_queries <- 10
N_list <- Poncatime:::generate_data_interface(N_data, N_queries, 10)
count <- with(N_list, Poncatime:::asoCurvatureEstimation_interface(points, queries, 10))
