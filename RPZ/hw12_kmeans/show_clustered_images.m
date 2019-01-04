function show_clustered_images(images, labels)
% show_clustered_images(imgs, labels)
%
%   Shows results of clustering.
%
%   Parameters:
%       images - <w x h x n> set of images
%       labels - <1 x n> estimated labels for the images

l = unique(labels);
n = length(l);
wh = [ceil(n / round(sqrt(n))), round(sqrt(n))];
figure;

for i = 1:n
    imgs = images(:,:,labels == l(i));
    subplot(wh(1), wh(2), i);
    montage(permute(imgs, [1 2 4 3]));
    title(num2str(i));
end
    

