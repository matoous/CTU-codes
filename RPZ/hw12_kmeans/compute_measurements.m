function x = compute_measurements(images)
% x = compute_measurements(images)
%
% Describes images by the two measurements:
%
% x1 = (sum of pixel values in the left half of image)
%     -(sum of pixel values in the right half of image)
%
% x2 = (sum of pixel values in the upper half of image)
%     -(sum of pixel values in the lower half of image)
%
% Input:
%   images    .. Images to be described, of size [h, w, number_of_images].
%
% Output:

%   x         .. Measurements of the input images, of size [2, number_of_images],
%                where the first and the second row contain measurements x1
%                and x2 respectively.

% Measurement x1
sumRows = sum(double(images));
x1 = sum(sumRows(1, 1:floor(size(sumRows, 2)/2), :)) ...
   - sum(sumRows(1, floor(size(sumRows,2)/2)+1:end, :));
x1 = reshape(x1, 1, size(x1, 3));

% Measurement x2
sumCols = sum(double(images), 2);
x2 = sum(sumCols(1:floor(size(sumCols, 1)/2), 1, :)) ...
   - sum(sumCols(floor(size(sumCols, 1)/2)+1:end, 1, :));
x2 = reshape(x2, 1, size(x2, 3));

x = [x1; x2];
