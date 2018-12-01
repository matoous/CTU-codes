function show_mnist_classification(imgs, labels)
% show_mnist_classification(imgs, labels)
%
%   Shows results of MNIST digits classification.
%
%   Parameters:
%       imgs - <(w x h) x n> set of images
%       labels - <1 x n> estimated labels for the images

nImages = size(imgs, 2);
images = zeros(28, 28, nImages);
for i = 1 : nImages
    images(:, :, i) = reshape(imgs(:, i), [28, 28])';
end;

figure;
subplot(1, 2, 1)
montage(permute(images(:,:, labels == -1), [1, 2, 4, 3])); 
title('0', 'FontSize', 16);

subplot(1, 2, 2)
montage(permute(images(:,:, labels == 1), [1, 2, 4, 3])); 
title('1', 'FontSize', 16);
