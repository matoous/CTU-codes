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
    if w(1)*tst(1,i) + w(2)*tst(2,i) + w(3)*tst(1,i)^2 + w(4)*tst(1,i)*tst(2,i)+w(5)*tst(2,i)^2 + b > 0
        K(i) = 1;
    else
        K(i) = 2;
    end
end
end