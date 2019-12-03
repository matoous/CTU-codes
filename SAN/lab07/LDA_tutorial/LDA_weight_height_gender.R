## illustrate LDA
## available in MASS library

## weight, height, gender example
## show the analogy between LDA and its interpretation through Bayes formula

library(MASS)

# https://www.kaggle.com/yersever/500-person-gender-height-weight-bodymassindex
# not a convenient dataset, men and women of the same height
# life.data<-read.csv("500-person-gender-height-weight-bodymassindex.csv")
# https://www.kaggle.com/mustafaali96/weight-height
# height in inches, weight in pounds, transform to cm and kg
life.data<-read.csv("weight-height.csv")
life.data$Height<-life.data$Height*2.54
life.data$Weight<-life.data$Weight*0.454

# 1. work with height only
# visualize the data
boxplot(Height~Gender,life.data)
plot(density(life.data$Height[life.data$Gender=="Female"]),col="blue")
lines(density(life.data$Height[life.data$Gender=="Male"]),col="red")
# estimate params, substitute into Bayes formula
# men and women normal distribution parameters
mu.w<-mean(life.data$Height[life.data$Gender=="Female"])
mu.m<-mean(life.data$Height[life.data$Gender=="Male"])
s.w<-sd(life.data$Height[life.data$Gender=="Female"])
s.m<-sd(life.data$Height[life.data$Gender=="Male"])
s.all<-(s.m+s.w)/2 # LDA makes the assumption of equal variance
# calculate the posterior for Male, simplify knowing P(Male)=P(Female)
# Pr(Male|Heigh)=Pr(Heigh|Male)/(Pr(Heigh|Male)+Pr(Heigh|Female))
# postM<-dnorm(life.data$Height,mu.m,s.m)/(dnorm(life.data$Height,mu.m,s.m)+dnorm(life.data$Height,mu.w,s.w)) # "QDA like"
postM<-dnorm(life.data$Height,mu.m,s.all)/(dnorm(life.data$Height,mu.m,s.all)+dnorm(life.data$Height,mu.w,s.all)) # better matches LDA result
head(postM)
table(postM>0.5,life.data$Gender)
sum(diag(table(postM>0.5,life.data$Gender)))/nrow(life.data) # accuracy

# run LDA
lda_res<-lda(Gender~Height,life.data)
lda_res
# posteriors match those computed previously, very small deviations exist
head(predict(lda_res,life.data)$posterior)
table(predict(lda_res,life.data)$class,life.data$Gender)
sum(diag(table(predict(lda_res,life.data)$class,life.data$Gender)))/nrow(life.data) # the same accuracy

# understand the decision boundary in this 1D case
see.db<-data.frame(height=life.data$Height,pred=predict(lda_res,life.data)$class) # check that the decision boundary is the average of means
print(paste("Expected db:", (mu.w+mu.m)/2, "Observed db: ", (max(see.db[see.db$pred=="Female",1])+min(see.db[see.db$pred=="Male",1]))/2)) # match, the true db does not have to be in the mid between the highest woman and shortest man

plot(density(life.data$Height[life.data$Gender=="Female"]),lwd=2,col="blue",main="Decision boundary") # the real case first
lines(density(life.data$Height[life.data$Gender=="Male"]),lwd=2,col="red")
abline(v=(mu.w+mu.m)/2) # standard deviations averaged, db does not cross the intersection of class densities in the real case
scale<-seq(min(life.data$Height),max(life.data$Height),0.1)
lines(scale,dnorm(scale, mean=mu.w, sd=s.all),type="l",lty="dotted",col="blue") # the ideal case then, normal approximation
lines(scale,dnorm(scale, mean=mu.m, sd=s.all),lty="dotted",col="red")

# 2. work both with height and weight
# visualize the data
plot(Weight~Height,life.data,col=c("blue","red")[Gender])
# estimate params, substitute into Bayes formula
# men and women normal distribution parameters
library(mvtnorm)
mu.w<-colMeans(life.data[life.data$Gender=="Female",c(2,3)])
mu.m<-colMeans(life.data[life.data$Gender=="Male",c(2,3)])
cov.w<-cov(life.data[life.data$Gender=="Female",c(2,3)])
cov.m<-cov(life.data[life.data$Gender=="Male",c(2,3)])
cov<-(cov.w+cov.m)/2 # could be oversimplification?
# guess the posterior for the first 5 samples, simplify knowing P(Male)=P(Female)
# Pr(Male|Heigh,Weight)=Pr(Heigh,Weight|Male)/(Pr(Heigh,Weight|Male)+Pr(Heigh,Weight|Female))
postM<-dmvnorm(life.data[c(2,3)],mu.m,cov)/(dmvnorm(life.data[c(2,3)],mu.m,cov)+dmvnorm(life.data[c(2,3)],mu.w,cov))
head(postM)
table(postM>0.5,life.data$Gender)
sum(diag(table(postM>0.5,life.data$Gender)))/nrow(life.data) # accuracy, by 8% better than with Height only

# run LDA
lda_res<-lda(Gender~.,life.data)
lda_res
head(predict(lda_res,life.data)$posterior) # posteriors perfectly match those computed previously
table(predict(lda_res,life.data)$class,life.data$Gender)
sum(diag(table(predict(lda_res,life.data)$class,life.data$Gender)))/nrow(life.data) # the same accuracy, by 8% better than with Height only

source("decision_plot.R")
decisionplot(lda_res, life.data, class = "Gender", main = "LDA",resolution=200) # plot decision boundary

# run QDA
qda_res<-qda(Gender~.,life.data)
qda_res
table(predict(qda_res,life.data)$class,life.data$Gender)
sum(diag(table(predict(qda_res,life.data)$class,life.data$Gender)))/nrow(life.data) # the same train accuracy as LDA, more freedom does not help here
decisionplot(qda_res, life.data, class = "Gender", main = "QDA",resolution=200) # decision boundary close to linear
qda_res$scaling # both the genders scale nearly equally (scaling transforms the observations so that within-group covariance matrices are spherical)
var(as.matrix(subset(life.data, Gender=="Male",2:3)) %*% qda_res$scaling[,,"Male"]) # after scaling, the group variances equal 1 and covarinaces 0

# run logistic regression
# start with the simple Height model, to be able to plot the sigmoid curve
lr<-glm(Gender~Height,life.data,family=binomial)
lr
lr.prob<-predict(lr,life.data,type="response") # probability of Men for train data
lr.pred<-rep("Female",length(lr.prob))
lr.pred[lr.prob>0.5]<-"Male"
table(lr.pred,life.data$Gender)
sum(diag(table(lr.pred,life.data$Gender))/nrow(life.data)) # the train accuracy for LDA was 0.8328
plot(life.data$Height,as.numeric(life.data$Gender)-1,xlab="Height",ylab="P(Gender=Male|Height)")
points(life.data$Height,lr.prob,col="red")
abline(v=(max(life.data$Height[lr.pred=="Female"])+min(life.data$Height[lr.pred=="Male"]))/2,col="blue") # LR decision boundary
abline(h=0.5,col="blue")
# learn both with height and weight
lr<-glm(Gender~.,life.data,family=binomial)
lr
lr.prob<-predict(lr,life.data,type="response") # probability of Men for train data
lr.pred<-rep("Female",length(lr.prob))
lr.pred[lr.prob>0.5]<-"Male"
table(lr.pred,life.data$Gender)
sum(diag(table(lr.pred,life.data$Gender))/nrow(life.data)) # the train accuracy for LDA was 0.9193
class(lr) <- c("lr", class(lr)) # a bit ajust the lr model for plotting the decision boundary
predict.lr <- function(object, newdata, ...)
  predict.glm(object, newdata, type = "response") > .5
decisionplot(lr, life.data, class = "Gender", main = "Logistic regression",resolution=200) # the decision boundary is linear!

# independent work
# Which method is expected to work best on test data in this task (LDA, QDA or LR)? Answer without testing first.
# Experimentally verify your answer, remember that you may need to deal with smaller train sets to see any difference ...

# It is LDA that is supposed to work best in this task. Its assumptions are clearly met, QDA will tend to overfit while logistic regression will suffer from missing parametric assumtions.

# Verification
