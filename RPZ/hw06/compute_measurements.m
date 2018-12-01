function [x, norm_params] = compute_measurements(images, norm_params)
% x = compute_measurements(images)
%
%   Describes images by the following measurement:
%
%   x = (sum of pixel values in the left half of image)
%      -(sum of pixel values in the right half of image)
%
%   Input:
%       images - Images to be described, of size [w, h, number_of_images].
%       norm_params (optional) - Normalization parameters:
%                [mean of x, std. dev. of x]
%                (computed from input data if not present)
%
%   Return:
%       x - Measurements of the input images, of size [1, number_of_images]
%       norm_params - see above

% Measurement x
sumRows = sum(double(images));
x = sum(sumRows(1, 1:floor(size(sumRows, 2) / 2), :)) ...
  - sum(sumRows(1, floor(size(sumRows,2) / 2) + 1:end, :));
x = reshape(x, 1, size(x, 3));

% Normalization
if nargin < 2
    % If normalization parameters are not provided, compute it from data
    norm_params = [mean(x), std(x)];
end
x = (x - norm_params(1)) ./ norm_params(2);
