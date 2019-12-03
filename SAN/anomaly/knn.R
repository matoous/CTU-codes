# Implement anomaly detector based on k-nearest neighbor. You can use library FNN for finding
#nearest neighbours. The implementation should be stored in a file knn.r.

library(FNN)
get_knn <- function(data, k){
  knn = get.knn(data, k)
  knn.indices <- knn[[1]]
  knn.distances <- knn[[2]]
  return(knn)
}

# merges given anomalous and normal data together (data could be a list of any length)
merge_data <- function(data){
  merged = matrix(, nrow=0, ncol=3)
  for(idx in 1:length(data)){
    anomalous = data[[idx]]$anomalous
    normal = data[[idx]]$normal
    tmp <- rbind(anomalous, normal)
    merged <- rbind(merged, tmp)
  }
  
  return(merged)
}

# for each dataset (problem), tries many values of k and selects the best one
# returns dataframe where row corresponds to k values, columns to problems and each cell
# contains AUC value for given k and problem
find_optimal_k <- function(datasets){

  k_values = c(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,25,50)
  problems_df = data.frame(row.names = k_values)
  
  # iterate over all problems  
  for(i in 1:length(datasets)){
    optimal_k = 1
    data = datasets[i]
    
    # merge data of training datasets
    merged <- merge_data(data)
    # calculate ratio of anomalous/normal data
    anomalous_count = length(which(merged[,3]==1))
    normal_count = length(which(merged[,3]==0))
    p = anomalous_count/normal_count
    p_count = round(p*nrow(merged))
  
    auc_values = array()
    iter = 1
    # for some range of values of k
    for(k in k_values){
      # calculate distances to k neighbors
      knn <- get_knn(merged, k)
      dists <- knn$nn.dist
      # sort descending by distance to k-th neighbor
      sorted_desc <- order(dists[,k], decreasing = TRUE)
      # mark first 'p' (those with longest distances) as anomalous
      first_p = sorted_desc[1:p_count]
      anomalous_labels = rep(0,nrow(dists))
      anomalous_labels[first_p] = 1
      # calculate auc for given k
      auc_val = auc(roc(anomalous_labels, factor(merged[,3])))
      
      auc_values[iter] =  auc_val
      iter = iter + 1
    }
    optimal_k = which.max(auc_values)
    print(names(datasets[i]))
    
    problems_df = cbind(problems_df, auc_values)
   # problems_df = cbind(problems_df, auc_values)
  }
  colnames(problems_df) = names(datasets)
  return(problems_df)
}

