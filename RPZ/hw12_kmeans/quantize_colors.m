function im_q = quantize_colors(im, k)
% im_q = quantize_colors(im, k)
%
% Image color quantization using the k-means clustering. The pixel colors
% are at first clustered into k clusters. Color of each pixel is then set
% to the mean color of the cluster to which it belongs to.
%
% Input:
%   im        .. Image whose pixel colors are to be quantized.
%
%   k         .. Required number of quantized colors.
%
% Output:
%   im_q      .. Image with quantized colors.


% Convert the image from RGB to L*a*b* color space
cform = makecform('srgb2lab');
im = applycform(im, cform);


% YOUR CODE HERE


% Convert the image from L*a*b* back to RGB
cform = makecform('lab2srgb');
im_q = applycform(im_q, cform);
