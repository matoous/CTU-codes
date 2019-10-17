## Male Egyptian Skulls
## Brian S. Everitt and Torsten Hothorn: A Handbook of Statistical Analyses Using R
## solve the research questions below with the aid of various statistical methods:
## 1. are there any differences in the skull sizes between the time periods?
## 2. do they show any (gradual) changes with time?
## 3. how are the four measurements related?

library(HSAUR) # install the library before

data("skulls", package = "HSAUR") # load a dataset from a package into the workspace, not necessary here
help(skulls) # see the info about the dataset
summary(skulls) # see the basic stats

## visualize the dataset

# improve feature visualization, introduce a concised text label
skulls$labels<-as.character(skulls$epoch)
skulls$labels[skulls$labels=="cAD150"]<-"c0150AD"
skulls$labels[skulls$labels=="c200BC"]<-"c0200BC"
skulls$labels<-as.factor(sapply(skulls$labels,function(x) paste(substr(x,2,3),substr(x,6,6),sep="")))

means <- aggregate(skulls[,c("mb", "bh", "bl", "nh")],list(epoch = skulls$epoch), mean) # calculate means of the continuos varaibles for the individual epochs
means
# look at these means graphically
pairs(means[,-1], panel = function(x, y) {text(x, y, abbreviate(levels(skulls$labels)))}) # plot a scatterplot matrix of epoch means for Egyptian skulls data
savePlot("skulls_mean_pairs.png",type="png")

# employ boxplots for 1D analysis
boxplot(mb ~ epoch,skulls) # this could be repeated 4 times, or ... use ggplot to construct a single graph
require(reshape2)
require(ggplot2)
# melting by epoch
df.skulls <- melt(skulls, id.var = "epoch")
ggplot(data = df.skulls, aes(x=variable, y=value)) + geom_boxplot(aes(fill=epoch))
p <- ggplot(data = df.skulls, aes(x=variable, y=value)) + geom_boxplot(aes(fill=epoch))
p + facet_wrap( ~ variable, scales="free")

# 3d scatterplots can be informative too, but means seem to be better here
library(scatterplot3d)

m <- matrix(c(1,2,3,4,5,5),nrow = 3,ncol = 2,byrow = TRUE)
layout(mat = m,heights = c(0.4,0.4,0.2))
mar0<-c(2,3,2,3)
  
scatterplot3d(skulls$mb, skulls$bh, skulls$bl, mar = mar0, color = rainbow(5)[skulls$epoch], pch = 19)
scatterplot3d(skulls$bh, skulls$bl, skulls$nh, mar = mar0, color = rainbow(5)[skulls$epoch], pch = 19)
scatterplot3d(skulls$mb, skulls$bl, skulls$nh, mar = mar0, color = rainbow(5)[skulls$epoch], pch = 19)
scatterplot3d(skulls$mb, skulls$bh, skulls$nh, mar = mar0, color = rainbow(5)[skulls$epoch], pch = 19)
  
plot(1, type = "n", axes=FALSE, xlab="", ylab="")
plot_colors <- rainbow(5)
legend("top",legend=levels(skulls$epoch), col = rainbow(5),pch=19,horiz=TRUE)

# dimension reduction, PCA
get_pca_var<-function(transf){
  # get eigenvalues
  ev<-transf$sdev^2
  ev<-ev/sum(ev)
  print(paste(round(100*(ev[1]+ev[2])),"% variance explained in the first two principal components.",sep=""))
}  

library(ellipse)
pca<-prcomp(skulls[c(2:5)],scale=T)
get_pca_var(pca) # the model explains 64% variance
X11(type="cairo")
plot(pca$x[,1:2],col=rainbow(5)[skulls$epoch],pch=19,lwd=3,main="Dimension reduction, PCA")
legend("bottomleft",legend=levels(skulls$epoch),col=rainbow(5),pch=19,box.lwd=0)

biplot(pca) # shows variable collibearity too

plotEllipse<-function(pcares,etype,level=0.95,color="black"){
  lines(ellipse(x=var(pcares$x[skulls$epoch==etype,1:2]),centre=colMeans(pcares$x[skulls$epoch==etype,1:2]),level=level),col=color)
}
mapply(plotEllipse,etype=levels(skulls$epoch),color=rainbow(5),MoreArgs=list(pcares=pca,level=0.9))
savePlot("PCA.png",type="png")

## cluster analysis
library(mclust)
# Run the function to see how many clusters it finds to be optimal
# set it to search for at least 1 model and up 10
d_clust <- Mclust(as.matrix(skulls[c(2:5)]), G=1:10)
m.best <- dim(d_clust$z)[2]
cat("model-based optimal number of clusters:", m.best, "\n")
plot(d_clust) # 1 cluster only

d_dist <- dist(as.matrix(skulls[c(2:5)]))   # find distance matrix 
hc<-hclust(d_dist)
plot(hc,labels=skulls$epoch)
# color leaves
library(dendextend)
dend <- as.dendrogram(hc,leaflab=skulls$epoch)
labels_colors(dend) <- rainbow(5)[skulls$epoch][order.dendrogram(dend)]
# Plotting the new dendrogram
plot(dend)
legend("topright",legend=levels(skulls$epoch),col=rainbow(5),pch=19,box.lwd=0)

## MANOVA
# there appear to be quite large differences between the epoch means, use MANOVA to test
# before you do so, test the assumtions, the first is normal within-group distribution, 
# alternatively, no outliers that are most dangerous
library(MVN) # MultiVariate Normal
# the proposed outliers should be removed
lapply(levels(skulls$epoch),function(x) mvOutlier(subset(skulls,epoch==x)[c(2:5)],qqplot=TRUE,method="adj.quan"))
library(biotools)
# the Box’s M-test for homogeneity of covariance matrices (see Rencher's book Methods of Multivariate Analysis for details)
boxM(skulls[c(2:5)], skulls$epoch)
# illustrative plot
source('my_plotcorr.R')
corr.1 <- cor(skulls[skulls$epoch=="c4000BC",c(2:5)])
corr.2 <- cor(skulls[skulls$epoch=="cAD150",c(2:5)])
colsc=c(rgb(241, 54, 23, maxColorValue=255), 'white', rgb(0, 61, 104, maxColorValue=255))
colramp = colorRampPalette(colsc, space='Lab')
colors = colramp(100)
# then pick colors along this 100 point scale given the correlation value * 100 rounded down to the nearest integer
# to do that we need to move the correlation range from [-1, 1] to [0, 100]
# now plot again with ellipses along the diagonal
my.plotcorr(corr.1, col=colors[((corr + 1)/2) * 100], diag='ellipse', lower.panel="number", main='4000BC')
my.plotcorr(corr.2, col=colors[((corr + 1)/2) * 100], diag='ellipse', lower.panel="number", main='AD150')

skulls_manova <- manova(cbind(mb, bh, bl, nh) ~ epoch, data = skulls)

# perform different tests and see the p-values
summary(skulls_manova, test = "Pillai")
summary(skulls_manova, test = "Wilks")
summary(skulls_manova, test = "Hotelling-Lawley")
summary(skulls_manova, test = "Roy")

# the simple t-tests show much lower power
t.test(skulls$bl[skulls$epoch=="c4000BC"],skulls$bl[skulls$epoch=="cAD150"])
# auxilliary figure
X11(type="cairo")
p1 <- hist(skulls$bl[skulls$epoch=="c4000BC"],breaks=c(80,85,90,95,100,105,110,115))
p2 <- hist(skulls$bl[skulls$epoch=="cAD150"],breaks=c(80,85,90,95,100,105,110,115))
plot( p1, col=rgb(0,0,1,1/2), xlim=c(80,120),ylim=c(0,15),main="basialveolar length",xlab="bl")
plot( p2, col=rgb(1,0,0,1/2), xlim=c(80,120), add=T)
legend("topright", c("c4000BC","cAD150"), lwd=4, col=c(rgb(0,0,1,1/2), rgb(1,0,0,1/2)))
savePlot("t-test-hist.png",type="png")

# The p-values associated with each four test criteria are very small and there is strong evidence that the skull measurements differ between the five epochs
# investigate which epochs differ and on which variables, employ the univariate F-tests for each of the four variables
summary.aov(skulls_manova)
# We see that the results for the maximum breadths (mb) and basialiveolar length (bl) are highly significant
# with those for the other two variables, in particular for nasal heights (nh), suggesting little evidence of a difference

# To look at the pairwise multivariate tests (any of the four test criteria are equivalent in the case of a one-way layout with two levels only) we can use the summary method and manova function as follows
summary(manova(cbind(mb, bh, bl, nh) ~ epoch, data = skulls, subset = epoch %in% c("c4000BC", "c3300BC")))
summary(manova(cbind(mb, bh, bl, nh) ~ epoch, data = skulls, subset = epoch %in% c("c4000BC", "c1850BC")))
summary(manova(cbind(mb, bh, bl, nh) ~ epoch, data = skulls, subset = epoch %in% c("c4000BC", "c200BC")))
summary(manova(cbind(mb, bh, bl, nh) ~ epoch, data = skulls, subset = epoch %in% c("c4000BC", "cAD150")))

# To keep the overall significance level for the set of all pairwise multivariate tests under some control (and still maintain a reasonable power),
# Stevens (2001) recommends setting the nominal level alpha= 0.15 and carrying out each test at the alpha/m level where m is the number of tests performed
# the results of the four pairwise tests suggest that as the epochs become further separated in time the four skull measurements become increasingly distinct

## LDA
# perform LDA to compare -- two different packages and approaches
# see https://pbil.univ-lyon1.fr/R/pdf/course5.pdf for details

# the predictive approach first
# the  between  covariance  matrix is divided by the within covariance matrix
library(MASS)
dis1 <- lda(labels ~ mb+bh+bl+nh, skulls)
names(dis1)
dis1
plot(dis1, col = rainbow(5)[skulls$epoch])
# prediction is far from perfect even for train data
t1 <- table(skulls$labels,predict(dis1, skulls)$class)
sum(diag(t1))/nrow(skulls) # accuracy

# A discriminant analysis looks for combinations of variables maximizing the between covariance matrix divided by the total covariance matrix
library(ade4)
pca2 <- dudi.pca(skulls[, 2:5], scannf = FALSE)
dis2 <- discrimin(pca2, skulls$epoch, scannf = FALSE)
names(dis2)
dis2
plot(dis2)
# prediction is far from perfect even for train data
t2 <- table(skulls$epoch,predict(dis2, skulls)$class)
sum(diag(t2))/nrow(skulls) # accuracy

# test ordered differences among classes
# ordinal alternative of Kruskall-Wallis test
library(clinfun)
jonckheere.test(skulls$bl,skulls$epoch,nperm=1000)
jonckheere.test(skulls$mb,skulls$epoch)
jonckheere.test(skulls$bh,skulls$epoch)
jonckheere.test(skulls$nh,skulls$epoch)

## multivariate linear regression
skulls$year<-c(rep(-4000,30),rep(-3300,30),rep(-1850,30),rep(-200,30),rep(150,30))
skulls_lm <- lm(year~mb+bh+bl+nh,data=skulls)
summary(skulls_lm,corr=TRUE) # show correlataions among parameters too, the same as e.g., cor(skulls$bh,skulls$bl)
anova(skulls_lm) # analyzes the contribution of the individual parameters, but it is not marginal but sequential ... http://stats.stackexchange.com/questions/28938/why-do-linear-regression-and-anova-give-different-p-value-in-case-of-consideri
                 # i.e., lm tests whether beta=0 (provided that the given var is added to the rest of vars), while F-test is sequential (it takes the first var, then the second etc., only for the given order)
anova(lm(year~bl+mb+bh+nh,data=skulls)) # different result, bl suddenly irrelevant

# construct a reduced model
skulls_lm_small <- lm(year~mb+bl-1,data=skulls)
summary(skulls_lm_small) # surprisingly, it has a much better R^2, but the formulas that R takes with and without intercept are not comparable, see the link below
                         # http://stats.stackexchange.com/questions/26176/removal-of-statistically-significant-intercept-term-increases-r2-in-linear-mo
skulls_lm_small <- lm(year~mb+bl,data=skulls)
summary(skulls_lm_small)

# 3d plot for 2 vars and the response
s3d <- scatterplot3d(x=skulls$mb, y=skulls$bl, z=skulls$year, highlight.3d=TRUE, pch=19)
# Add regression plane
s3d$plane3d(skulls_lm_small, lty="solid")  # add the regression plane from model 1 above
observed.3d <- s3d$xyz.convert(skulls$mb, skulls$bl, skulls$year)  # locations of data points
predicted.3d <- s3d$xyz.convert(skulls$mb, skulls$bl, fitted(skulls_lm_small)) # corresponding locations on the regression plane
segments(observed.3d$x, observed.3d$y, predicted.3d$x, predicted.3d$y, lty="dashed")  # draw lines from data points to plane
savePlot("lm_res_fit.png",type="png")

# residual plot
# the residual plot suggests that the assumptions were not met
X11(type="cairo")
plot(skulls_lm$fitted,skulls_lm$resid,xlab="Fitted",ylab="Residuals")
# add trendline
ma <- function(x,n=5){filter(x,rep(1/n,n), sides=2)}
lines(sort(skulls_lm$fitted),ma(skulls_lm$resid[order(skulls_lm$fitted)],50))
loess(resid ~ fitted,data.frame(fitted=sort(skulls_lm$fitted),resid=skulls_lm$resid[order(skulls_lm$fitted)]))
lines(sort(skulls_lm$fitted), predict(loess_fit), col = "red")
lines(smooth.spline(sort(skulls_lm$fitted),skulls_lm$resid[order(skulls_lm$fitted)],spar=1),col="red")
savePlot("lm_res_plot.png",type="png")
plot(skulls$year,skulls_lm$resid)

## FACTOR analysis
# search for a general skull factor, corresponding e.g to brain size ...
factanal(skulls[c(2:5)],1)
