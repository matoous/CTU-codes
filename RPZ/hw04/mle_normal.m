function [mu sigma] = mle_normal(x)
% [mu sigma] = mle_normal(x)
%
%   Computes maximum likelihood estimate of mean and sigma of a normal
%   distribution.
%
%   Parameters:
%       x - input features <1 x n>
%
%   Returns:
%       mu - mean
%       sigma - standard deviation
mu = sum(x) / length(x);
sigma = sqrt(sum((x - mu) .^2) / length(x));