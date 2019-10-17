# setwd('/home/XY/Documents/SAN/1_cv_san/')

# 1) Create pie charts showing election attendence (voters vs. non-voters) for all lower house elections 
# (years 96, 98, 02 and 06)

electionData <- read.csv("GlobalElections_Czech elections.csv")
electionLabels <- read.csv("labels.csv")

lower_house_elections <- electionData[electionData$legid == 1,]

# What does this function do? Why does it does that? 
votes_total <- aggregate(lower_house_elections$dtv, by=list(lower_house_elections$year), FUN=sum)
# To see hint, run the commented command below
# View(lower_house_elections)

# Prepare placeholders for 4 pie charts
par(mfrow = c(2, 2))

for(i in  1:nrow(votes_total)){
  # TODO your code here
  
  pie(x = ..., labels = ...)
}

# 2) Create a chart showing election results (percentage of votes for each qualified party)
# for year 2006
# ------------------------------------------------------------------------------------
lh_2006 <- lower_house_elections[lower_house_elections$year == 2006,]
ps_cols <- grepl("p[0-9]*s", colnames(lower_house_elections))
total_ps <- apply(lh_2006[ps_cols], MARGIN = 2, FUN = sum, na.rm = TRUE)
total_ps <- total_ps[total_ps > 0]

electionLabels <- read.csv("labels.csv")

# TODO extract party names to party_names variable

names(total_ps) <- party_names

barplot(total_ps, main="Volebni vysledky 2006", xlab="Strana")
