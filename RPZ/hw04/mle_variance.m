function [var_mean var_sigma] = mle_variance(cardinality)
% [var_mean var_sigma] = mle_variance(cardinality)
%
%   Estimates variance of estimated parameters of a normal distribution 
%   in 100 trials.
%
%   Parameters:
%       cardinality - size of the generated dataset (e.g. 1000)
%   Returns
%       var_mean - variance of the estimated means in 100 trials
%       var_sigma - variance of the estimated standard deviation in 100 trials


    numTrials = 100;
    muRec = zeros(1,numTrials);
    sigmaRec = zeros(1,numTrials);
    
    for i = 1:numTrials
        [muRec(i) sigmaRec(i)] = mle_normal(randn(1, cardinality));
    end
    [x var_mean] = mle_normal(muRec);
    [x var_sigma] = mle_normal(sigmaRec);
    var_mean = var_mean^2;
    var_sigma = var_sigma^2;