N <- 8
N_list <- Poncatime:::generate_data_interface(N, N*100, 10)
asoCurvatureEstimation=with(N_list, data.frame(count=Poncatime:::asoCurvatureEstimation_interface(pointVec, queriesVec, 1)))
