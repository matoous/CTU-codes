function lr_histogram = compute_lr_histogram(letter_char, Alphabet, images, labels, num_bins)
% lr_histogram = compute_lr_histogram(letter_char, Alphabet, images, labels, num_bins)
%
%   Calculates feature histogram.
%
%   Parameters:
%       letter_char is a character representing the letter whose feature 
%                   histogram we want to compute, e.g. 'C'
%       Alphabet - string of characters
%       images - images in 3d matrix of shape <h x w x n>
%       labels - labels of images, indices to Alphabet list, <1 x n>
%       num_bins - number of histogram bins
%   
%   Return: 
%       lr_histogram - counts of values in the corresponding bins, vector <1 x num_bins>
%
%   class support: integer classes
    letter_images = images(:,:, find(labels==find(letter_char==Alphabet)));
    lefts = letter_images(:,1:end/2,:);
    rights = letter_images(:,end/2+1:end,:);
    tmp = reshape(sum(sum(lefts)) - sum(sum(rights)), [1, size(lefts, 3)]);
    lr_histogram = hist(tmp, num_bins)
end
