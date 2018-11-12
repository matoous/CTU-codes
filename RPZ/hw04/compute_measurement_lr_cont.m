function x = compute_measurement_lr_cont(imgs)
% x = compute_measurement_lr_cont(imgs)
%
%   Compute measurement on images, subtract sum of right half from sum of
%   left half.
%
%   Parameters:
%       imgs - set of images, <h x w x n>
%
%   Return:
%       x - measurements, <1 x n>

width = size(imgs(:,:,1), 2);

sumRows = sum(double(imgs));
x = sum(sumRows(1,1:(width/2),:)) - sum(sumRows(1,(width/2+1):width,:));
x = reshape(x,1,size(imgs,3));
