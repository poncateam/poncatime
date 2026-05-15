N_data <- 10000
N_queries <- 1000
dataScale <- 10
points <- matrix(nrow=N_data, ncol=6);
queries <- matrix(nrow=N_queries, ncol=3);
Poncatime:::generate_data_interface(points, queries, 10)
count <- Poncatime:::asoCurvatureEstimation_interface(points, queries, 10)
