% Assignment: The AdaBoost algorithm

%% init
%run('../../../../solutions/3rdparty/stprtool/stprpath.m')

% the positive class, the other digits will be used as negative samples
digit = 0;

%% load data
load('data_33rpz_cv07.mat');

img_size = 13;

Xtrn = reshape(trn_data.images, img_size*img_size, []);
ytrn = -ones(1,size(Xtrn,2));
ytrn(trn_data.labels == digit) = 1;

Xtst = reshape(tst_data.images, img_size*img_size, []);
ytst = -ones(1,size(Xtst,2));
ytst(tst_data.labels == digit) = 1;

%% run AdaBoost learning
num_steps = 30;
[strong_class wc_error upper_bound] = adaboost(Xtrn, ytrn, num_steps);

% compute the test error for all lengths of the strong classifier
train_error = compute_error(strong_class, Xtrn, ytrn);
test_error = compute_error(strong_class, Xtst, ytst);

%% plot errors
figure;
% training error
plot(train_error, 'b');
hold on;
% test error
plot(test_error, 'r');
% wc error
plot(wc_error, 'k');		
% upper bound
plot(upper_bound, 'g');		
% figure completion  
xlabel('training step');
ylabel('error');
legend('Training error', 'Test error', 'WC error', ...
       'Upper bound: cumprod(Zt)', ...
       'Location', 'NorthEast');
grid on;

%% show the classification results
yab = adaboost_classify(strong_class, Xtst);

show_classification(tst_data.images, yab, [num2str(digit) 'X']);

%% visualise the selected weak classifiers
% Create mean image of the color class
avgImage = mean(trn_data.images(:, :, ytrn == 1), 3);

% Convert to a color (3 channel) image
avgImage = repmat(avgImage, [1 1 3]); 

% Add mask to red channel, making the pixels corresponding to weak
% classifiers red (the intensity of red color corresponds to the classifier
% weight alpha)
mask = reshape(avgImage, [img_size*img_size 3]);
max_alpha = max(strong_class.alpha);
for I = 1:length(strong_class.wc)
    if strong_class.wc(I).parity == 1
        mask(strong_class.wc(I).idx, 1) = strong_class.alpha(I) ./ max_alpha;
        mask(strong_class.wc(I).idx, 2:3) = 0;
    else
        mask(strong_class.wc(I).idx, 2) = strong_class.alpha(I) ./ max_alpha;
        mask(strong_class.wc(I).idx, [1,3]) = 0;
    end
end
mask = reshape(mask, [img_size img_size 3]);
figure;
imshow(mask);







