function classify = adaboost_classify(strong_class, X)
% classify = adaboost_classify(strong_class, X)
%
% Applies the strong classifier to the data X and returns the
% classification labels
%
%   Parameters:
%       strong_class - the structure returned by adaboost()
%
%       X [K x N] - training samples, K is the number of weak classifiers
%            and N the number of data points
%
%   Returns:
%       classify [1 x N] - the labels of the input data X as predicted by
%            the strong classifier strong_class
%
N = size(X,2);
T = size(strong_class.wc,2);
alphas = strong_class.alpha;
wcs = strong_class.wc;
classify = zeros(1,N);

for i = 1:N
    % individual classifications by weak classifiers
    classif = arrayfun(@(j) alphas(j) * sign(wcs(j).parity * (X(wcs(j).idx,i) - wcs(j).theta)), 1:T);
    classify(i) = sign(sum(classif));
end
