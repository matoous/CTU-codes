function [ classif ] = classif_kernel_svm( X, model )
%CLASSIF_KERNEL_SVM performs classification on X by trained SVM classifier stored in model
% 
% Input:
% X         [n x m (double)] matrix containing feature points in columns
% model     [1 x 1 (struct)] structure with fields (.b, .fun, .sv, .y, .alpha, .options see my_kernel_svm.m for details)
% 
% Output:
% classif   [1 x m (double)] vector with labels for X obtained by classification 
% 
fx = (model.alpha'.*model.y)*getKernel(model.sv, X, model.options) + model.b;
classif = (fx >= 0) - (fx < 0);
end

