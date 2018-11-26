function Z = lift_dimension(X)
    
% Lifts the dimensionality of the feature space from 2 to 5 dimensions
%
%   Parameters:
%       X - training samples in the original space (2xN matrix)
%
%   Returns:
%       Z - training samples in the lifted feature space (5xN vector)
Z = [X; X(1,:).^2; X(1,:).*X(2,:); X(2,:).^2]
end