function centers = k_meanspp(x, k)
% KMEANSPP - perform k-means++ initialization for k-means clustering.
%
% Input:
%   X - [DIMS x N] N input vectors of dimenionality DIMS
%   K - [int] number of k-means centers
%
% Output:
%   CENTERS - [DIMS x K] K proposed centers for k-means initialization

% Number of vectors
N = size(x, 2);

% YOUR CODE HERE