function K = classif_quadrat_perc(tst, model)

% Classifies test samples using the quadratic discriminative function 
%
%   Parameters:
%       tst - samples for classification in original space (2xN matrix)
%       model - structure with the trained perceptron classifier
%       (parameters of the descriminative function)
%           model.w - weights vector (5x1 vector)
%           model.b - bias term (1 double)
%
%   Returns:
%       K - classification result (1xN vector, contains either 1 or 2)
w = model.w;
b = model.b;
for i = 1:size(tst,2)
    t = tst(:,i);
    K(i) =  double(w(1)*t(1) + w(2)*t(2) + w(3)*t(1)^2 + w(4)*t(1)*t(2)+w(5)*t(2)^2 + b > 0) + 1;
end
end