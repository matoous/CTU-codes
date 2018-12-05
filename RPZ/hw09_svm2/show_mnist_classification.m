function show_mnist_classification( X, y )
%SHOW_MNIST_CLASSIFICATION Summary of this function goes here
%   Detailed explanation goes here
    
    nImages = size(X, 2);
    images = zeros(28, 28, nImages);
    for i = 1 : nImages
        images(:, :, i) = reshape(X(:, i), [28, 28])';
    end;

    figure;
    subplot(1, 2, 1)
    montage(permute(images(:,:, y == -1), [1, 2, 4, 3])); 
    title('class 0', 'FontSize', 18);

    subplot(1, 2, 2)
    montage(permute(images(:,:, y == 1), [1, 2, 4, 3])); 
    title('class 1', 'FontSize', 18);
    
end

