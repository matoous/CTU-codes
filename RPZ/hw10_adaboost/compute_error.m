function errors = compute_error(strong_class, X, y)
% errors = compute_error(strong_class, X, y)
%
% Computes the error on data X for *all lengths* of the given strong
% classifier
%
%   Parameters:
%       strong_class - the structure returned by adaboost()
%
%       X [K x N] - samples, K is the number of weak classifiers and N the
%            number of data points
%
%       y [1 x N] - sample labels (-1 or 1)
%
%   Returns:
%       errors [1 x T] - error of the strong classifier for all lenghts 1:T
%            of the strong classifier
%
N = size(X,2);
M = size(strong_class.wc,2);
errors = zeros(1,M);
alphas = strong_class.alpha;
wcs = strong_class.wc;
for t = 1:length(strong_class.wc)
    for i = 1:N
        % individual classifications by weak classifiers
        classif = arrayfun(@(j) alphas(j) * sign(wcs(j).parity * (X(wcs(j).idx,i) - wcs(j).theta)), 1:t);
        errors(t) = errors(t) + (sign(sum(classif)) ~= y(i));
    end
    errors(t) = errors(t) / N;
end
