function show_classification(imgs, labels, letters)
% show_classification(imgs, labels, letters)
%
%   Shows results of classification.
%
%   Parameters:
%       imgs - <w x h x n> set of images
%       labels - <1 x n> estimated labels for the images
%       letters - string of length 2 with letters for labels
%                 e.g. 'CN'

imgs1 = imgs(:,:,labels == 1);
imgs2 = imgs(:,:,labels == -1);
figure;
subplot(1,2,1), montage(permute(imgs1, [1 2 4 3])), title(letters(1));
subplot(1,2,2), montage(permute(imgs2, [1 2 4 3])), title(letters(2));
