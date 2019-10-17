############################ 3.Tutorial ######################################

# We will use a well known mtcars dataset
?mtcars
# get names of each variables or columns 
head(mtcars)
# Let students choose attribute to look into
names(mtcars)

# Suppose we choose weights 
mtcars$wt <- mtcars$wt * 0.4535 * 1000

# Let's see what we can learn from the data and compute some statistics. 
# curb weight of Octavia - how would it stand with cars back then?
quantile(mtcars$wt)
percentile <- ecdf(mtcars$wt)
percentile(1650)

# Boxplot relates to quantiles (are basically their visualizations) and are useful for instance to visualize outliers
boxplot(mtcars$wt)

# Another useful statistic apart from quantiles is variance or standart deviation respectively
# What are they and how are they calculated?
# Effects in normal distribution?
var(mtcars$wt)
sd(mtcars$wt)
plot(mtcars$wt)
abline(h=mean(mtcars$wt))
abline(h=mean(mtcars$wt) + sd(mtcars$wt), lty=2)
abline(h=mean(mtcars$wt) - sd(mtcars$wt), lty=2)

# Plotting single points as above is nice, but seeing distribution of the points is 
# a bit more informative
hist(mtcars$hp, breaks=10)
d <- density(mtcars$mpg)
plot(d)  # what can we tell from the shape of the curve?
polygon(d, col="red")

# Now lets look at effects between more then one attribute
# What is covariance and correlation? How are the calculated?
cor(mtcars$hp, mtcars$mpg)
cov(mtcars$hp, mtcars$mpg)
# Let students interpret the results of correlation matrix. Interesting relationships?
cor(mtcars)

# --------------------------------- GRAPHS ----------------------------------------------------------
# dot plots
# Simple Dotplot
dotchart(mtcars$mpg,labels=row.names(mtcars),cex=.7, main="Gas Milage for Car Models",
         xlab="Miles Per Gallon")

# Sort by mpg, group and color by cylinder
x <- mtcars[order(mtcars$mpg),] # sort by mpg
x$cyl <- factor(x$cyl) # it must be a factor
levels(x$cyl)
x$color[x$cyl==4] <- "red"
x$color[x$cyl==6] <- "blue"
x$color[x$cyl==8] <- "darkgreen"
dotchart(x$mpg,labels=row.names(x),cex=.7,groups= x$cyl,
         main="Gas Milage for Car Models\ngrouped by cylinder",
         xlab="Miles Per Gallon", gcolor="black", color=x$color)


# bar plots
# Simple Bar Plot
counts <- table(mtcars$gear)
barplot(counts, main="Car Distribution",
        xlab="Number of Gears")

# Stacked Bar Plot with Colors and Legend
counts <- table(mtcars$vs, mtcars$gear)
barplot(counts, main="Car Distribution by Gears and VS",
        xlab="Number of Gears", col=c("darkblue","red"),
        legend = rownames(counts))

# Grouped Bar Plot
counts <- table(mtcars$vs, mtcars$gear)
barplot(counts, main="Car Distribution by Gears and VS",
        xlab="Number of Gears", col=c("darkblue","red"),
        legend = rownames(counts), beside=TRUE)

#*Bar plots need not be based on counts or frequencies. You can create bar plots that represent means, medians, standard deviations, etc. Use the aggregate( ) function and pass the results to the barplot( ) function.

# pie charts
# Simple Pie Chart
slices <- c(10, 12,4, 16, 8)
lbls <- c("US", "UK", "Australia", "Germany", "France")
pie(slices, labels = lbls, main="Pie Chart of Countries")

# Pie Chart with Percentages
slices <- c(10, 12, 4, 16, 8)
lbls <- c("US", "UK", "Australia", "Germany", "France")
pct <- round(slices/sum(slices)*100)
lbls <- paste(lbls, pct) # add percents to labels
lbls <- paste(lbls,"%",sep="") # ad % to labels
pie(slices,labels = lbls, col=rainbow(length(lbls)),
    main="Pie Chart of Countries") 

# 3D Exploded Pie Chart
library(plotrix)
slices <- c(10, 12, 4, 16, 8)
lbls <- c("US", "UK", "Australia", "Germany", "France")
pie3D(slices,labels=lbls,explode=0.1,
      main="Pie Chart of Countries ")

# Pie Chart from data frame with Appended Sample Sizes
mytable <- table(iris$Species)
lbls <- paste(names(mytable), "\n", mytable, sep="")
pie(mytable, labels = lbls,
    main="Pie Chart of Species\n (with sample sizes)") 

# boxplots
# Boxplot of MPG by Car Cylinders
boxplot(mpg~cyl,data=mtcars, main="Car Milage Data",
        xlab="Number of Cylinders", ylab="Miles Per Gallon") 
# Notched Boxplot of Tooth Growth Against 2 Crossed Factors
# boxes colored for ease of interpretation
#boxplot(len~supp*dose, data=ToothGrowth, notch=TRUE,
#        col=(c("gold","darkgreen")),
#        main="Tooth Growth", xlab="Suppliment and Dose") 


# scatterplots
# Simple Scatterplot
attach(mtcars)
plot(wt, mpg, main="Scatterplot Example",
     xlab="Car Weight ", ylab="Miles Per Gallon ", pch=19) 

# Add fit lines
abline(lm(mpg~wt), col="red") # regression line (y~x)
lines(lowess(wt,mpg), col="blue") # lowess line (x,y) 

# Enhanced Scatterplot of MPG vs. Weight
# by Number of Car Cylinders
library(car)
scatterplot(mpg ~ wt | cyl, data=mtcars,
            xlab="Weight of Car", ylab="Miles Per Gallon",
            main="Enhanced Scatter Plot",
            labels=row.names(mtcars)) 

# Basic Scatterplot Matrix
pairs(~mpg+disp+drat+wt,data=mtcars,
      main="Simple Scatterplot Matrix")

# Scatterplot Matrices from the lattice Package
library(lattice)
splom(mtcars[c(1,3,5,6)], groups=cyl, data=mtcars,
      panel=panel.superpose,
      key=list(title="Three Cylinder Options",
               columns=3,
               points=list(pch=super.sym$pch[1:3],
                           col=super.sym$col[1:3]),
               text=list(c("4 Cylinder","6 Cylinder","8 Cylinder")))) 

# Scatterplot Matrices from the car Package
library(car)
scatterplot.matrix(~mpg+disp+drat+wt|cyl, data=mtcars,
                   main="Three Cylinder Options")


### R Markdown - authoring framework for data science
#R Markdown
browseURL("http://rmarkdown.rstudio.com")
#Cheatsheet
browseURL("https://www.rstudio.com/wp-content/uploads/2016/03/rmarkdown-cheatsheet-2.0.pdf")
#Reference Guide
browseURL("https://www.rstudio.com/wp-content/uploads/2015/03/rmarkdown-reference.pdf")

install.packages('rmarkdown')
library(rmarkdown)
