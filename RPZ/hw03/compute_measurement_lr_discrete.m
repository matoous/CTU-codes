function x = compute_measurement_lr_discrete(imgs)
% x = compute_measurement_lr_discrete(imgs)
%   
%   Calculates difference between left and right half of image(s).
%
%   Parameters:
%       imgs - set of images, <h x w x n> or <h x w x 3 x n> (color images)
%   
%   Returns: 
%       x - vector of values in range <-10, 10>, 
%           shape <1 x n>
%
%   class support: integer classes

mu = -563.9;
sigma = 2001.6;

% size of the images
height = size(imgs, 1);
width = size(imgs, 2);
%count = size(imgs, 3);

% works even for matrix of color images (4 dimensions)
x_raw = squeeze( sum(sum(imgs(:,1:(width/2),:,:))) - sum(sum(imgs(:,(width/2+1):width,:,:))) );

x = round((x_raw - mu) / (2 * sigma) * 10);
x(x > 10) = 10;
x(x < -10) = -10;
