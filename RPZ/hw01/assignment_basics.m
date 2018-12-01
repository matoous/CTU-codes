
A = [16     2     3    13;
      5    11    10     8;
      9     7     6    12;
      4    14    15     1]
 
B = [ 3     4     9     4     3     6     6     2     3     4;
      9     2    10     1     4     3     7     1     3     5]

output = matrix_manip(A,B)

%%

load data_33rpz_cv01.mat

ims = permute(images,[1 2 4 3]); % The images must be ordered by the 4th dimension.
montage(ims); colormap gray;

initials = 'MD'

letter_mean1 = compute_letter_mean(initials(1), Alphabet, images, labels);
letter_mean2 = compute_letter_mean(initials(2), Alphabet, images, labels);

imwrite(letter_mean1, 'initial1_mean.png')
imwrite(letter_mean2, 'initial2_mean.png')

f = figure;
feature_histogram1 = compute_lr_histogram(initials(1), Alphabet, images, labels, 10)
hold on
feature_histogram2 = compute_lr_histogram(initials(2), Alphabet, images, labels, 10)
saveas(f, 'initials_histograms.png')
