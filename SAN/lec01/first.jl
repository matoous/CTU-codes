using DataFrames, StatsBase

x = 3:30
y = x[findall(x -> x >= 10, x)]
describe(y);
println(quantile(y, 0:.25:1));
println(map(x -> x ^ 2, 1:10))
z = (1:10) .^ 2 .* 2
println(map(x -> x * 2, z))
println(mean_and_std(z))
