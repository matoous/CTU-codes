function x = wages_fit_model(t, M)
% function x = wages_fit_model(t, M)
%
% INPUT: N data points specified by
% t : N-by-1 vector, years
% M : N-by-1 vector, wages
%
% OUTPUT:
% x: 2-by-1 vector specifying the estimated model 
% M(t) = x[1] + x[2]*t

% discard the code from here and implement the functionality:

% \ operator: Solve a simple system of linear equations, A*x = B. 
% for us the B is the target = the wage for given year
% left side for us is x0 + x1*t

x = [ones(size(t)) t] \ M;