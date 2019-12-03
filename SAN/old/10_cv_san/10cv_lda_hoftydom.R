# hoftydom
# 15.12.18


#################### Linear Discriminant Analysis ##################
rm(list = ls())
#Compute within-scatter matrix
ComputeWithinScatter <- function(data, n)
{
  withinMatrix <- 0
  for(i in 1:n){
    withinMatrix <- withinMatrix + cov(as.matrix(data[[i]]))
  }
  return(withinMatrix)
}

#Compute between-scatter matrix
ComputeBetweenScatter <- function(data, n, meanOverall)
{
  betweenMatrix <- 0
  for(i in 1:n){
    class_mean <- colMeans(as.matrix(data[[i]]))
    betweenMatrix <- betweenMatrix + nrow(data[[i]])*as.matrix((class_mean - meanOverall)%*%t(class_mean-meanOverall))
  }
  return(betweenMatrix)
}

#Solve the EigenProblem and return eigen-vector
SolveEigenProblem <- function(withinMatrix, betweenMatrix, prior)
{
  # ADD YOUR CODE
  return(eigen(solve(withinMatrix) %*% betweenMatrix))
}

ComputeCentroids <- function(data, labels){
  yGroupedMean <- aggregate(as.data.frame(data), by = list(labels), FUN = mean)
  rownames(yGroupedMean) <- yGroupedMean[,1]
  yGroupedMean <- yGroupedMean[,-1]
  return(yGroupedMean)
}

Classify <- function(newData, eigenVectors, labels, centroids){
  y <- as.matrix(newData) %*% eigenVectors[,1:(length(levels(labels))-1)]
  prior <- table(labels)/sum(table(labels))
  
  classification <- matrix(nrow = nrow(newData), ncol = length(levels(labels)))
  colnames(classification) <- levels(labels)
  for(c in levels(labels))
  {
    classification[,c] <- as.matrix(0.5*rowSums((y - matrix(rep(as.matrix(centroids[c,]),
                                                                nrow(newData)), nrow = nrow(newData),
                                                            byrow = TRUE) )^2)
                                    - log(prior[c]))
  }
  return(levels(labels)[apply(classification, MARGIN = 1, which.min)])
}

CrossvalidationLDA <- function(mydata, labels, kfolds = 10){
  set.seed(17)
  #randomly shuffle the data
  random <- sample(nrow(mydata))
  data <-mydata[random,]
  labels <- labels[random]
  #Create 10 equally size folds
  folds <- cut(seq(1,nrow(data)),breaks=kfolds,labels=FALSE)
  acc <- rep(0, times = kfolds)
  #10 fold cross validation
  for(i in 1:kfolds){
    #Segment your data by fold using the which() function 
    testIndexes <- which(folds==i,arr.ind=TRUE)
    testData <- data[testIndexes, ]
    trainData <- data[-testIndexes, ]
    testLabels <- labels[testIndexes]
    trainLabels <- labels[-testIndexes]
    
    eigenLDA <- LDA(trainData, trainLabels)
    centroids <- ComputeCentroids(as.matrix(trainData) %*% eigenLDA[,1:(length(levels(trainLabels))-1)],
                                  labels = trainLabels)
    pre <- Classify(newData = testData, labels = trainLabels, eigenVectors = eigenLDA,
                    centroids = centroids)
    acc[i] <- sum(pre == testLabels)/length(testLabels)
  }
  return(mean(acc))
}

LDA <- function(mydata, labels){

  #number of classes
  n <-length(levels(labels))

  # 1) split the data w.r.t. given factors
  splittedData <- split(mydata, labels)
  
  # 2) scatter matrices
  #############  within-class scatter matrix Sw ##################
  withinScatterMatrix <- ComputeWithinScatter(splittedData, n)
  withinScatterMatrix
  #############  between-class scatter matrix Sb ##################
  betweenScatterMatrix <- ComputeBetweenScatter(splittedData,n, colMeans(as.matrix(mydata)))
  betweenScatterMatrix
  # 3)  eigen problem
  ############ solve Eigen problem ################################
  ei <- SolveEigenProblem(withinScatterMatrix, betweenScatterMatrix)
  
  #transform the samples onto the new subspace
  y <- (as.matrix(mydata) %*% ei$vectors[,1:2])
  
  ## visual comparison with PCA
  par(mfrow=c(1,2))
  # pca <- prcomp(mydata,scale. = TRUE)
  pca <- prcomp(mydata)
  plot(y[,1], y[,2], col = labels, pch = 21, lwd = 2, xlab = "LD1" , ylab = "LD2", main = "LDA")
  plot(-pca$x, col = labels, pch = 21, lwd = 2, main = "PCA")

  return(ei$vectors)
}

############################# FUNCTIONS END ###################################


############################# MAIN ##########################################

### PREPARE DATA
#data(iris)
#mydata <- iris
#labels <- mydata[,5]
#mydata <- mydata[,-5]

setwd("~/Dropbox/CVUT/MGR/1. semestr/SAN/10_cv_san")

mydata <- read.csv("wine.csv", header = FALSE)
labels <- mydata[,1]
labels <- as.factor(labels)
mydata <- mydata[,-1]

#compute LDA and return corresponding eigenvectors
eigenLDA <- LDA(mydata, labels)
#find centroids in the transformed data
centroids <- ComputeCentroids(as.matrix(mydata) %*% eigenLDA[,1:(length(levels(labels))-1)],
                              labels = labels)
#make predictions on the "mydata"
prediction <- Classify(newData = mydata, labels = labels, eigenVectors = eigenLDA,
         centroids = centroids)
#ACC
sum(prediction == labels)/(length(labels))

#CrossValidation
accLDA <- CrossvalidationLDA(mydata, labels, kfolds = 10)


# DISCUSS
# LDA vs PCA:
## As can be seen in the plots, LDA managed to split the data into 3 classes compared to PCA,
## that didn't manage to classify the data well. The accuracy of
## LDA is 98.3% which is quite good and might be considered as succesfull result. 
# Scaled vs. Unscaled
## Unscaled PCA, as already described above, didn't manage to classify the data well. On the 
## other hand, when 'scale. = TRUE' parameter was used, the result of PCA dramatically increased
## and almost reached the same success as LDA.