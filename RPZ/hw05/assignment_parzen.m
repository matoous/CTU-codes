% Assigment: PARZEN WINDOW ESTIMATION

%% init
load data_33rpz_parzen.mat
run('/home/matouus/statistical-pattern-recognition-toolbox-master/stprpath.m')

%% Tasks, part 1
%% measurements
x = compute_measurement_lr_cont(trn.images);

% splitting the trainning data into classes
xA = x(trn.labels==1);
xC = x(trn.labels==2);

%% computing the histograms of training data
[bins_A centers_A] = hist(xA, 20);
bins_A = bins_A / (sum(bins_A)*(centers_A(2)-centers_A(1)));

[bins_C centers_C] = hist(xC);
bins_C = bins_C / (sum(bins_C)*(centers_C(2)-centers_C(1)));

%% estimating conditional probability using Parzen window
x_range = min(xA):100:max(xA);
h = [100 500 1000 2000];
p = my_parzen(x_range,xA,h);

%% plots of the estimates
figure;
% h = 100
subplot(2,2,1);
bar(centers_A, bins_A);
hold on;
plot(x_range, p(1,:), 'r', 'linewidth', 2);
title('h = 100');
xlabel('x');
bins_C = bins_C / (sum(bins_C)*(centers_C(2)-centers_C(1)));

%% estimating conditional probability using Parzen window
x_range = min(xA):100:max(xA);
h = [100 500 1000 2000];
p = my_parzen(x_range,xA,h);

%% plots of the estimates
figure;
% h = 100
subplot(2,2,1);
bar(centers_A, bins_A);
hold on;
plot(x_range, p(1,:), 'r', 'linewidth', 2);
title('h = 100');
xlabel('x');
ylabel('p(x|A)');
ylim([0 4.5e-4]);
grid on;
% h = 300
subplot(2,2,2);
bar(centers_A, bins_A);
hold on;
plot(x_range, p(2,:), 'r', 'linewidth', 2);
title('h = 500');
xlabel('x');
ylabel('p(x|A)');
ylim([0 4.5e-4]);
grid on;
% h = 1000
subplot(2,2,3);
bar(centers_A, bins_A);
hold on;
plot(x_range, p(3,:), 'r', 'linewidth', 2);
title('h = 1000');
xlabel('x');
ylabel('p(x|A)');
ylim([0 4.5e-4]);
grid on;
% h = 3000
subplot(2,2,4);
bar(centers_A, bins_A);
hold on;
plot(x_range, p(4,:), 'r', 'linewidth', 2);
title('h = 2000');
xlabel('x');
ylabel('p(x|A)');
ylim([0 4.5e-4]);
grid on;

%% Tasks, part 2
%% 10-fold cross-validation init
h_range = 100:50:1000;
num_folds = 10;

%% class A cross-validation
rand('seed', 42);   % needed only for upload system, to test the correctness of the code

[train_data test_data] = crossval(length(xA),num_folds);

Lh = compute_Lh(train_data, test_data, xA, h_range);

%% optimal value of parameter h
h_bestA = h_range(Lh==max(Lh))
Lh_bestA = max(Lh)

%% plots of optimal h
figure;
subplot(1,2,1);
plot(h_range, Lh);
hold on;
plot(h_bestA, Lh_bestA,'or');
cur_ylim = get(gca, 'ylim');
plot([h_bestA h_bestA], [cur_ylim(1) Lh_bestA], '--r');
title('10-fold cross-validation');
xlabel('h');
ylabel('L(h)');
grid on;

p = my_parzen(x_range, xA, h_bestA);
subplot(1,2,2);
bar(centers_A, bins_A);
hold on;
plot(x_range, p, 'r', 'linewidth', 2);
grid on;
title('Best bandwidth h for class A');
xlabel('x');
ylabel('p(x|A)');

%% class C cross-validation
rand('seed', 42);   % needed only for upload system, to test the correctness of the code

[train_data test_data] = crossval(length(xC),num_folds);

Lh = compute_Lh(train_data, test_data, xC, h_range);

%% optimal value of parameter h
h_bestC = h_range(Lh==max(Lh))
Lh_bestC = max(Lh)

%% plots of optimal h
figure;
subplot(1,2,1);
plot(h_range, Lh);
hold on;
plot(h_bestC, Lh_bestC,'or');
title('10-fold cross-validation');
xlabel('h');
ylabel('L(h)');
grid on;

p = my_parzen(x_range, xC, h_bestA);
subplot(1,2,2);
bar(centers_C, bins_C);
hold on;
plot(x_range, p, 'r', 'linewidth', 2);
grid on;
title('Best bandwidth h for class C');
xlabel('x');
ylabel('p(x|C)');

%% Bayesian classifier
x_test = compute_measurement_lr_cont(tst.images);

% computing a priori probabilities
pA = estimate_prior(trn.labels, 1);
pC = estimate_prior(trn.labels, 2);

labels = classify_bayes_parzen(x_test, xA, xC, pA, pC, h_bestA, h_bestC);

% showing images
show_classification(tst.images, labels, 'AC'); 

% classification error
bayes_error = sum(tst.labels ~= labels) / length(labels)
bayes_error_perc = bayes_error * 100
