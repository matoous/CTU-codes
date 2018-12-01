function g = logistic_loss_gradient(X, y, w)
% g = logistic_loss_gradient(X, y, w)
%
%   Calculates gradient of the logistic loss function.
%
%   Parameters:
%       X - d-dimensional observations of size [d, number_of_observations]
%       y - labels of the observations of size [1, number_of_observations]
%       w - weights of size [d, 1]
%
%   Return:
%       g - resulting gradient vector of size [d, 1]
g = 0;
for i = 1:length(y)
    g = g + (1 / (1 + exp(y(i)*w'*X(:,i)))) * (X(:,i)*y(i));
end
g = -g / length(y);