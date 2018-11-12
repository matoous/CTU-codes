function y = classify_images(X, w)
% y = classify_images(X, w)
%
%   Classification by logistic regression.
%
%   Parameters:
%       X - d-dimensional observations of size [d, number_of_observations]
%       w - weights of size [d, 1]
%
%   Return:
%       y - estimated labels of the observations of size [1, number_of_observations]
y = w' * X;
y = (y > 0) - (y <= 0);