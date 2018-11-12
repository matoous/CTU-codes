function [L maximizerSigma maxL] = loglikelihood_sigma(x, D, sigmas)
% [L maximizerSigma maxL] = loglikelihood_sigma(x, D, sigmas)
%
%   Compute log ligelihoods and maximum ll of a normal
%   distribution with fixed mean and variable standard deviation.
%
%   Parameters:
%       x - input features <1 x n>
%       D - D.Mean the normal distribution mean
%       sigmas - <1 x m> vector of standard deviations
%
%   Returns:
%       L - <1 x m> vector of log likelihoods
%       maximizerSigma - sigma for the maximal log likelihood
%       maxL - maximal log likelihood
% no need of for loops!!!
customLog = @(y) log(1/(y*sqrt(2*pi)));
customS = @(y) (1/(2*y^2))*(x-D.Mean).^2;
L = arrayfun(@(y) sum(customLog(y) - customS(y)), sigmas);
maximizerSigma = fminbnd(@(sigmas) - sum(customLog(sigmas) - customS(sigmas)),0,max(sigmas));
maxL = sum(customLog(maximizerSigma) - customS(maximizerSigma));

