%% K-means Assignment

%run('../../stprtool/stprpath.m')
rng(0); % Initialization of seed for random number generation

%% Load data
load image_data.mat % Images of letters
load mnist_image_data.mat % Images of numbers from the MNIST dataset
im = imread('geeks.png');

%% K-means (HLT letters)
disp('K-means, K=3...');
x = compute_measurements(images);
k = 3;
[c, means, ~] = k_means(x, k, Inf, true);
show_clusters(x, c, means);
show_clustered_images(images, c);
show_mean_images(images, c);

%% K-means (numbers)
disp('K-means, K=10...');
x = compute_measurements(mnist_images);
k = 10;
[c, means, ~] = k_means(x, k, Inf, true);
show_clusters(x, c, means);
show_clustered_images(mnist_images, c);
show_mean_images(mnist_images, c);

%% Avoid Local Minima
disp('K-means with multiple trials...');
x = compute_measurements(images);
k = 3;
[c_m, means_m, x_dists_m] = k_means_multiple_trials(x, k, 10, Inf, false);
show_clusters(x, c_m, means_m);
show_clustered_images(images, c_m);

%% random initialization (try this block multiple times)
disp('K-means, K=4...');
x = gen_kmeanspp_data();
k = 4;
means = x(:, randsample(size(x, 2), k));
show_clusters(x, ones(1, size(x, 2)), means);
title('K-means initialization (random)');
disp('Press any key or mouse button in the figure...')
waitforbuttonpress; 
print('random_initialization.png','-dpng')
[c, means, ~] = k_means(x, k, Inf, false, means);
show_clusters(x, c, means);
title('K-means results (random initialization)');

%% K-means++ initialization  (try this block multiple times)
disp('K-means++, K=4...');
x = gen_kmeanspp_data();
k = 4;
means = k_meanspp(x, k);
show_clusters(x, ones(1, size(x, 2)), means);
title('K-means initialization (K-Means++)');
disp('Press any key or mouse button in the figure...')   
waitforbuttonpress;     
print('kmeanspp_initialization.png','-dpng')
[c, means, ~] = k_means(x, k, Inf, false, means);
show_clusters(x, c, means);
title('K-means results (K-means++ initialization)');

%% K-means Application - Color Quantization
im_q = quantize_colors(im, 8);
figure;
imshow(im_q);
print('geeks_quantized.png','-dpng')
