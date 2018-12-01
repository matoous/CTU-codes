function [ classif ] = classif_lin_svm( X, model )
%CLASSIF_LIN_SVM performs classification on X by trained SVM classifier stored in model
%   
% Input:
% X 		[n x m (double)] matrix containing feature points in columns
% model 	[1x1 (struct)] structure with fields .W and .b
% Output:
% classif 	[1 x m (double)] labels for X obtained, by classification
for i = 1:size(X,2)
    if model.W'*X(:,i) + model.b >= 0
        classif(i) = 1;
    else
        classif(i) = -1;
    end
end
end

