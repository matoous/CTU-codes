library('AUC')

# select best k
select_best_k <- function(datasets_k_values){
  # one way - means of values
  datasets_rowmeans = rowMeans(datasets_k_values)
  ordered_by_value = order(datasets_rowmeans, decreasing = TRUE)
  optimal_k_by_value = row.names(datasets_k_values[ordered_by_value[1],])
  
  # another way 
  # datasets_ranked <- apply(datasets_k_values, 2, function(x){order(x, decreasing = TRUE)})
  datasets_ranked <- apply(datasets_k_values, 2, function(x){rank(x, ties.method = "average")})
  datasets_ranked_rowmeans = rowMeans(datasets_ranked)
  ordered_by_rank = order(datasets_ranked_rowmeans, decreasing = TRUE)
  optimal_k_by_rank = row.names(datasets_k_values[ordered_by_rank[1],])
  #return(as.numeric(optimal_k_by_value))
  return(as.numeric(optimal_k_by_rank))
}

# evaluates knn detector on n-th dataset using k-value that was selected as best on the n-1 datasets
# returns dataframe where row corresponds to problems and column corresponds to anomaly method 
# (only knn detector in my case..), each cell contains AUC value for optimal k
evaluate_knn <- function(problems, k_values){

  problems_auc_dataframe = data.frame(row.names = names(problems))
  problems_auc_dataframe[,"knn"] <- NA
  
  # iterate through all problems, select optimal value of k from
  # n-1 problems, and evaluate it on the n-th problem
  for(i in 1:length(problems)){
    test_problem = problems[i]
    # select k from n-1 problems
    optimal_k = select_best_k(k_values[-i])
    
    merged = merge_data(test_problem)
    
    # calculate ratio of anomalous/normal data
    anomalous_count = length(which(merged[,3]==1))
    normal_count = length(which(merged[,3]==0))
    p = anomalous_count/normal_count
    p_count = round(p*nrow(merged))
    
    source("knn.R")
    # calculate distances to k neighbors
    knn <- get_knn(merged, optimal_k)
    
    # sort descending by distance to k-th neighbor
    sorted_desc <- order(knn$nn.dist[,optimal_k], decreasing = TRUE)
    # mark first 'p' (those with longest distances) as anomalous
    first_p = sorted_desc[1:p_count]
    anomalous_labels = rep(0,length(sorted_desc))
    anomalous_labels[first_p] = 1
    # calculate auc for given k
    auc_val = auc(roc(anomalous_labels, factor(merged[,3])))
    problems_auc_dataframe[i,"knn"] <- auc_val
  }
  
  return(problems_auc_dataframe)
}

