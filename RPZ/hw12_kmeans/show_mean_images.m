function show_mean_images(images, c)
% show_avarage_images(images, c)
%
% Shows mean images of each image cluster.
%
% Input:
%   images    .. Input images, of size [w, h, number_of_images].
%
%   c         .. Cluster index for each feature vector, of size
%                [1,number_of_images], containing only values from 1 to k,
%                i.e. c(i) is the index of a cluster which the image
%                images(:,:,i) belongs to.

l = unique(c);
n = length(l);
wh = [ceil(n / round(sqrt(n))), round(sqrt(n))];
figure;

for i = 1:n
    mean_image = mean(images(:, :, c==l(i)), 3);
    
    % Visualization
    subplot(wh(1), wh(2), i);
    imshow(mean_image, []);
    title(sprintf('cluster %d mean', l(i)));
end
