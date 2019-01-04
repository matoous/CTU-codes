function [c, means, sq_dists] = k_means(x, k, max_iter, show, init_means)
% [c, means, sq_dists] = k_means(x, k, max_iter, show, init_means)
%
% Implementation of the k-means clustering algorithm.
%
% Input:
%   x         .. Feature vectors, of size [dim,number_of_vectors], where dim
%                is arbitrary feature vector dimension.
%
%   k         .. Required number of clusters (single number).
%
%   max_iter  .. Stopping criterion: max. number of iterations (single number).
%                Set it to Inf if you wish to deactivate this criterion. 
%
%   [show]      .. Boolean switch to turn on/off visualization of partial
%                  results, optional.
%   [init_means] .. [dim, k] initial cluster prototypes, optional
%
% Output:
%   c         .. Cluster index for each feature vector, of size
%                [1, number_of_vectors], containing only values from 1 to k,
%                i.e. c(i) is the index of a cluster which the vector x(:,i)
%                belongs to.
%
%   means     .. Cluster centers, of size [dim,k], i.e. means(:,i) is the
%                center of the i-th cluster.
%
%   sq_dists   .. Squared distances to the nearest mean for each feature vector,
%                of size [1, number_of_vectors].
%
%   Note 1: All inputs and outputs are of double type.
%
%   Note 2: The iterative procedure terminates if either maximum number of
%   iterations is reached or there is no change in assignment of data to the
%   clusters.

if nargin < 4
    show = false;
end

% Number of vectors
N = size(x, 2);
c = zeros(1, N);

% Means initialization
if nargin < 5
    ind = randsample(N, k);
    means = x(:, ind);
else
    means = init_means;
end

i_iter = 0;
while i_iter < max_iter

    % YOUR CODE HERE



    
    % Ploting partial results
    if show
        fprintf('Iteration: %d\n', i_iter);
        show_clusters(x, c, means);
        disp('Press any key or mouse button in the figure...')    
        waitforbuttonpress;        
    end
end
        
end
