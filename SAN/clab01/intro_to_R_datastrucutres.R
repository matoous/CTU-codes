############################ 1.Tutorial ######################################
# R project
browseURL("https://www.r-project.org")

# R style guide
browseURL("https://google.github.io/styleguide/Rguide.xml")

# For Matlab users
browseURL("http://www.math.umaine.edu/~hiebeler/comp/matlabR.html")

# Lists of packages
browseURL("https://cran.r-project.org/")

# R reference card
browseURL("http://mirrors.nic.cz/R/doc/contrib/Short-refcard.pdf")

# R reference card for data mining
browseURL("http://mirrors.nic.cz/R/doc/contrib/YanchangZhao-refcard-data-mining.pdf")


########################### Data types
#numeric - integer, double, complex
#character
#logical
#vector
#factor
#matrix
#array
#list
#data.frame
#function

####################################

#basic stuff
8 + 5
4^2
print("hello world")

# VECTORS
#sequence of numbers with equal spread
1:12

seq(1, 12)
seq(1, 12, 0.5)
seq(1, 30, length = 40)

# or as concatenation of scalars (single elements)
c(6, 7, 8, 9, 10)
c('c', 'd', 'e')
c()

#variable
x <- 1:5
a <- c(6, 7, 8, 9, 10)
b <- scan()
rep(1, times = 10)
rep(c(1, 2, 3), times = 10)
rep(c(1, 2, 3), each = 10)
#vector math
x
a
x + a
x * 2

##MATRIX
matrix(c(1, 2, 3, 4, 5, 6, 7, 8, 9), nrow = 3)

row1 <- c(1, 2, 3)
row2 <- c(4, 5, 6)
row3 <- c(7, 8, 9)
matrix_from_rows <- rbind(row1, row2, row3)
matrix_from_rows

col1 <- c(1, 4, 9)
col2 <- c(2, 5, 8)
col3 <- c(3, 6, 9)
cbind(col1, col2, col3)

# rbind and cbind can be used for adding new rows/columns as well
matrix1 <- matrix(1:9, nrow = 3)
cbind(matrix1, col1)

matrix_from_rows[2, ]
matrix_from_rows[matrix_from_rows < 2] = 0 

##DATA FRAMES
# Multiple data types TODO
#Like matrices, but with named columns, which can be referenced without knowing their index
df <- data.frame(
  column1 = c(1, 4, 9),
  column2 = c(2, 5, 8),
  column3 = c(3, 6, 9)
)

area51raid <- data.frame(
  raider_name = c("Kyle", "Karen", "Kaenu Reaves", "Johny Sin"),
  max_proximity = c(12, 4, 0, 0),
  arrested = c(TRUE, TRUE, FALSE, FALSE),
  # ignore for now
  stringsAsFactors = FALSE
)

View(area51raid)

area51raid$max_proximity
area51raid$arrested

# Transform matrix to dataframe. What are the column names
as.data.frame(matrix_from_rows)

as.matrix(area51raid)  ## All cells converted to string values, bad for further computation
## -> can be treated by as.numeric(m[,2])
# vs.
data.matrix(area51raid)

## FACTORS ##
# First add a new column
area51raid$type <- c("naruto_run", "distraction", "elite", "elite")
area51raid
summary(area51raid)

# Introducing factors as the new column. Advantages?
area51raid$type <-
  factor(c("naruto_run", "distraction", "elite", "elite"))
summary(area51raid)
levels(area51raid$type)

barplot(table(area51raid$type))
as.numeric(area51raid$type)

# ordered factors and their perks
area51raid$type <-
  factor(
    c("naruto_run", "distraction", "elite", "elite"),
    levels = c("distraction", "naruto_run", "elite"),
    ordered = TRUE
  )
max(area51raid$type)
area51raid[area51raid$type < "elite",]

rbind(area51raid, c("Paul the Furry", 4, TRUE, 'distraction'))

##LISTS
data_list <-
  list(c("Mon", "Tue", "Wed"),
       matrix(c(1, 2, 3, 4,-1, 9), nrow = 2),
       list("Spotify", 12.3))
names(data_list) <- c("Weekdays", "Matrix", "Misc")
print(data_list)
data_list$Weekdays

## What happens when (continue with ?c to shown it creates both vector and list)
test <-
  c(c("Mon", "Tue", "Wed"),
    matrix(c(1, 2, 3, 4,-1, 9), nrow = 2),
    list("Spotify", 12.3))

# Predifined lists
letters
LETTERS
month.abb
month.name


matrix(cbind())