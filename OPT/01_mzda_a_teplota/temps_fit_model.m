function [x, omega] = temps_fit_model(t, T)
% function x = temps_fit_model(t, T)
%
% INPUT: N data points specified by
% t : N-by-1 vector, days
% T : N-by-1 vector, temperatures
%
% OUTPUT:
% x: 4-by-1 vector specifying the estimated model 
% T(t) = x[1] + x[2]*t + x[3]*sin(omega*t) + x[4]*cos(omega*t) 
%
% omega: a scalar. Set to a constant in the code, not estimated
%        from the data.
%

% discard the code from here and implement the functionality:
omega = 1/365*2*pi; 
x = [ones(size(t)) t arrayfun(@(x) sin(omega*x), t) arrayfun(@(x) cos(omega*x), t)] \ T;

