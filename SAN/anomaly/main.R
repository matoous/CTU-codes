# main script
# Dominik Hoftych, 7.1.2019

# reset working dir
rm(list=ls())

# NEED TO CHANGE YOUR WORKING DIRECTORY, otherwise 'list.dirs' method WONT WORK
# (or you can specify absolute path manually in 'list.dirs' method parameters)

# get all provided .csv files
# taken from
# https://stackoverflow.com/questions/4749783/how-to-obtain-a-list-of-directories-within-a-directory-like-list-files-but-i
list.dirs <- function(path="problems/", pattern=NULL, all.dirs=FALSE,
                      full.names=FALSE, ignore.case=FALSE) {
  # use full.names=TRUE to pass to file.info
  all <- list.files(path, pattern, all.dirs,
                    full.names=TRUE, recursive=FALSE, ignore.case)
  dirs <- all[file.info(all)$isdir]
  # determine whether to return full names or just dir names
  if(isTRUE(full.names))
    return(dirs)
  else
    return(basename(dirs))
}

problems = list()
iter = 1
problem_names = c()
# iterate over all directories and load .csv files
for(dir in list.dirs()){
  problem_names[iter] = dir
  file <- list.files(path = paste("problems", dir, sep="/"), pattern = "*.csv", full.names = TRUE, recursive = FALSE)
  anom = file[1]
  normal = file[2]
  # create a list of problems
  data_anomalous = as.matrix(read.csv(anom, header=TRUE))
  data_anomalous = cbind(data_anomalous, rep(1,nrow(data_anomalous)))
  colnames(data_anomalous) = c("x","y","label")
  data_normal = as.matrix(read.csv(normal, header=TRUE))
  data_normal = cbind(data_normal, rep(0,nrow(data_normal)))
  colnames(data_normal) = c("x","y","label")
  problem = list(data_anomalous, data_normal)
  names(problem) = c("anomalous", "normal")

  problems[iter] = list(problem)
  
  iter = iter + 1
}
names(problems) = problem_names

# find optimal k for each dataset 
source("knn.R")
datasets_k_values <- find_optimal_k(problems)
# evaluate it on each dataset using leave-one-out method
source("evaluation.R")
problems_auc_dataframe <- evaluate_knn(problems, datasets_k_values)

