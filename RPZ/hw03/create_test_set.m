function [images, labels] = create_test_set(images_test, letters)
% [images, labels] = create_test_set(images_test, alphabet, letters)
%
% Return subset of the <images_test> corresponding to <letters>.
%
%   parameters:
%       images_test - set of all test images
%                     images_test.A, images_test.B, ...
%       letters - string with letters, e.g. 'CN'
%
%   returns:
%       images - shape h x w x n
%       labels - shape 1 x n

all_images = {};
all_labels = {};

for i = 1:length(letters)
    eval(['imgs = images_test.' letters(i) ';']);
    all_images{end+1} = imgs;
    all_labels{end+1} = i * ones(1,size(imgs,3));
end

images = cat(3,all_images{:});
labels = cat(2,all_labels{:});

