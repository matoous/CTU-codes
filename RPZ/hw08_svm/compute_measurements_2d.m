function [ X, y, normalization ] = compute_measurements_2d( input_struct, normalization )
%COMPUTE_MEASUREMENTS_2D computes 2D features from image measurements
%   
% Input:
% input_struct 		    [1x1 (struct)] structure with fields 
%      			        .images [w x h x m (uint8)] image data 
%				        .labels [1 x m (double)] labels 
% normalization (optional) - computed from input data if not present
%                       [1 x 4 (double)] normalization constants
%                       [left right mean, lr std, up down mean, ud std]
%
% Output:
% X			        [2 x m (double)] matrix with feature vectors (each column is one feature vector)
% y 			    [1 x m (double)] vector with labels 
% normalization	    see above	
   
    images = input_struct.images;
   
    left = images(:, 1:end/2, :);       up = images(1:end/2, :, :);
    right = images(:, end/2+1:end, :);  down = images(end/2+1:end, :, :);
    L = sum(sum(left), 2); L = L(:);    U = sum(sum(up), 2); U = U(:);
    R = sum(sum(right), 2); R = R(:);   D = sum(sum(down), 2); D = D(:);
    a = L - R; b = U - D;

    % normalization
    if nargin < 2
		% if normalization parameter is not present, compute it from input data
		normalization = [...]  % use std function
    end
    a = ...
    b = ...
    X = [a'; b'];
    
    
    labels = input_struct.labels;    
    y = ...

end

