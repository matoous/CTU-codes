function labels = classify_bayes_parzen(x_test, xA, xC, pA, pC, h_bestA, h_bestC)
% labels = classify_bayes_parzen(x_test, xA, xC, pA, pC, h_bestA, h_bestC)
%
%   Classifies data using bayesian classifier with densities estimated by
%   Parzen window estimator.
%
%   Parameters:
%       x_test - data (measurements) to be classified
%       xA, xC - training data for Parzen window for class A and C
%       pA, PC - prior probabilities
%       h_bestA, h_bestC - optimal values of the kernel bandwidth
%
%   Returns:
%       labels - classification labels for x_test
probA = my_parzen(x_test, xA, h_bestA);
probC = my_parzen(x_test, xC, h_bestC);
mtx = [(probA*pA)' (probC*pC)'] * [0 1;1 0];
[A,B] = min(mtx,[],2);
labels = B';