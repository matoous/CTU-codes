function [w, b] = perceptron(X, y, maxIterations)
    
% Implements the perceptron algorithm
% (http://en.wikipedia.org/wiki/Perceptron)
%   
%   Parameters:
%       X - training samples (DxN matrix)
%       y - training labels (1xN vector, contains either 1 or 2)
%
%   Returns:
%       w - weights vector (Dx1 vector)
%       b - bias (offset) term (1 double)
w = zeros(1, size(X, 1) + 1);
X = [X; ones(1, size(X, 2))];
X(:,y == 2) = -X(:,y == 2);
for i = 1:maxIterations
    for j = 1:length(y)
        res = w * X(:,j);
        if res > 0
            if j == length(y)
                b = w(end);
                w = w(1:end-1)';
                return
            end
        else
            w = w + X(:,j)';
            break;
        end
    end
end
w = NaN;
b = NaN;
end