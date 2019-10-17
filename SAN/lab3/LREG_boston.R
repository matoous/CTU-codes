## Illustrate linear regression
## This script stems from the lab accompanying Chapter 3 of Introduction to Statistical Learning

library("MASS") # collection of data sets and functions
library("ISLR") # data sets associated with the text
library("glmnet") # lasso

## use Boston data set available in MASS to predict median house value in neighborhoods around Boston
help(Boston) # learn the structure of the dataset, 506 areas described by 14 variables, medv will serve as the target variable

## Simple linear regression
## Start with a basic regression using the lm(y ~ x, data) function
lm.fit <- lm(medv ~ lstat, data = Boston) # lstat gives the percent of households with low socioeconomic status in the neighborhood
summary(lm.fit) # see the model
names(lm.fit) # get all component names of the lm model
coef(lm.fit) # see the coefficeints once more, same as lm.fit$coefficients
confint(lm.fit) # confidence intervals around these estimates
predict(lm.fit, data.frame(lstat=c(5,10,15)), interval = "confidence") # predict for three new neighborhoods including confints

plot(Boston$lstat, Boston$medv) # visualize the relationship between indep and dep variable
abline(lm.fit, col="red", lw=2) # visualize the fit

par(mfrow=c(2,2))
plot(lm.fit) # see the diagnostic plots

plot(hatvalues(lm.fit)) # Leverage statistics can be computed for our predictors with hatvalues()
which.max(hatvalues(lm.fit)) # find the observation with the largest leverage

## Summary
# There is an obvious relationship between the independent variable (household status) and the dependent variable (median house value).
# Both the F-stat for the whole model and t-stat for the predictor are significant, the predictor helps to explain more than 50% of the variance in the target variable.
# The median house price is around $34500, on average it dercreases by $950 with one percent increase of househodls with low socioeconomical status. For more than 30% it virtually becomes $0 (the model does not extrapolate well for these unobserved values).
# Both simple plot and residual plot suggest that the relationship is rather non-linear.
# Consequences: prediction could be improved with additional non-linear terms, assumptions are violated, estimations do not have to be perfect (namely p-values, confidence intervals).

nlm.fit <- lm(medv ~ poly(lstat,2), data = Boston) # just see the improvement with the most simple polynomial regression
summary(nlm.fit) # explains around 10% variance more
plot(predict(nlm.fit), residuals(nlm.fit)) # residual plot looks much better (still not normal error distribution)
anova(lm.fit, nlm.fit) # the second model is significantly better, sum of sq is positive, p-value small

## Multiple linear regression
# Use multiple/all/relevant predictors at once, in a single model

lm.fit.all <- lm(medv ~ ., data=Boston) # employ all the available independent variables
summary(lm.fit.all) # model improves again, only some of the variables seem to be unimportant, we may want to exclude them
lm.fit.exclude <- lm(medv ~ . -age -indus, data=Boston) # truly exclude age and indus
summary(lm.fit.exclude) # the simpler model seems to maintain the performance of the previous model
anova(lm.fit.all,lm.fit.exclude) # no difference, the simpler model preferred

## Finally, try some feature selection methods
step <- stepAIC(lm.fit.all, direction="both") # stepwise regression, based on AIC criterion, taken from MASS library
step$anova # display results, actually does the same as we did manually, removes age and indus

# Fit LASSO to remove unimportant predictors
lambda_grid <- 10^ seq(10 , -3 , length =200)
lasso.model <- glmnet(as.matrix(Boston[,-ncol(Boston)]),Boston$medv,alpha=1, lambda=lambda_grid, standardize=TRUE)
lasso.cv.out <- cv.glmnet(as.matrix(Boston[,-ncol(Boston)]),Boston$medv,alpha=1)
plot(lasso.cv.out) # small lambda values preferred, shrinkage effect is small
lasso.lambda <- lasso.cv.out$lambda.min
lasso.coefficients <- predict(lasso.model, type="coefficients", s=lasso.lambda)

# Display the coefficients and selected variables
print("LASSO coefficients:")
print(as.matrix(lasso.coefficients)) # the absolute coefficent values influenced by the scale of the individual variables, nox has a small scale
print(as.matrix(lasso.coefficients)[seq(2,length(Boston)-1),] != 0) # removes indus and age too

## Summary
# To avoid overfitting and increase simplicity and understandability of the model, only relevant features should be used.
# We have shown three different approaches to feature selection: p-values, stepwise regression and lasso. All of them came to the same conclusion: two features should be removed from the model.
