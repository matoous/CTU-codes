% Assigment: MAXIMUM LIKELIHOOD ESTIMATION

% initialize stpr toolbox 
run('/home/matouus/statistical-pattern-recognition-toolbox-master/stprpath.m')

%% Part 1 of the task

stepCardinality = 10;
maxCardinality = 500;
cardinalities = 10:stepCardinality:maxCardinality;
n = size(cardinalities,2);

var_mu_rec = zeros(1,n);
var_sigma_rec = zeros(1,n);

for i = 1:n    
    % Compute the variance of the estimations for a fix cardinality
    [var_mu_rec(i) var_sigma_rec(i)] = mle_variance(cardinalities(i));    
end

figure, plot(cardinalities',[var_mu_rec' var_sigma_rec'],'lineWidth',3), grid on;
legend('Mean','Standard deviation');
xlabel 'Cardinality of traning set'
ylabel 'Variance'
saveas(gcf, 'mle_variances.png')

%% Part 2 of the task
%% Load training and test data
load data_33rpz_cv04.mat

%% Select the training set
trnSet = trn_2000;

%% Computing features vectors (trainning)
x = compute_measurement_lr_cont(trnSet.images);

%% Estimate prior probabilities
prior_A = estimate_prior(1,trnSet.labels);
prior_C = estimate_prior(2,trnSet.labels);

%% Splitting the trainning data into into classes
x_A = x(trnSet.labels == 1);
x_C = x(trnSet.labels == 2);

%% Computing Gaussian models of Maximal Likelihood
[DA.Mean DA.Sigma] = mle_normal(x_A);
[DC.Mean DC.Sigma] = mle_normal(x_C);

DA.Prior = prior_A;
DC.Prior = prior_C;

%% Plotting L VS sigma
sigmas = 200:1000;
[L, maximizerSigma, maxL] = loglikelihood_sigma(x_C, DC, sigmas);

% Plotting the likelihood as a function of Sigma
figure,plot(sigmas,L),grid on, title 'Likelihood varing sigma class C (trn\_2000)';
xlabel('\sigma');
ylabel('L(\sigma)');
hold on;
plot(maximizerSigma, maxL,'r+','markersize',15);
line([DC.Sigma DC.Sigma],[min(L) maxL],'Color','g');
hold off;
saveas(gcf,'loglikelihood2000.png');

%% Training set 200
x200 = compute_measurement_lr_cont(trn_200.images);

x_A200 = x200(trn_200.labels == 1);
x_C200 = x200(trn_200.labels == 2);

[DA200.Mean, DA200.Sigma] = mle_normal(x_A200);
[DC200.Mean, DC200.Sigma] = mle_normal(x_C200);
DA200.Prior = estimate_prior(1,trn_200.labels);
DC200.Prior = estimate_prior(2,trn_200.labels);
sigmas = 200:1000;
[L200, maximizerSigma200, maxL200] = loglikelihood_sigma(x_C200, DC200, sigmas);
figure,plot(sigmas,L200),grid on, title 'Likelihood varing sigma class C (trn\_200)';
xlabel('\sigma');
ylabel('L(\sigma)');
hold on;
plot(maximizerSigma200, maxL200,'r+','markersize',15);
line([DC200.Sigma DC200.Sigma],[min(L200) maxL200],'Color','g');
hold off;
saveas(gcf,'loglikelihood200.png');

%% Training set 20
x20 = compute_measurement_lr_cont(trn_20.images);

x_A20 = x20(trn_20.labels == 1);
x_C20 = x20(trn_20.labels == 2);

[DA20.Mean, DA20.Sigma] = mle_normal(x_A20);
[DC20.Mean, DC20.Sigma] = mle_normal(x_C20);
DA20.Prior = estimate_prior(1,trn_20.labels);
DC20.Prior = estimate_prior(2,trn_20.labels);
sigmas = 200:1000;
[L20, maximizerSigma20, maxL20] = loglikelihood_sigma(x_C20, DC20, sigmas);
figure,plot(sigmas,L20),grid on, title 'Likelihood varing sigma class C (trn\_20)';
xlabel('\sigma');
ylabel('L(\sigma)');
hold on;
plot(maximizerSigma20, maxL20,'r+','markersize',15);
line([DC20.Sigma DC20.Sigma],[min(L20) maxL20],'Color','g');
hold off;
saveas(gcf,'loglikelihood20.png');


%% Ploting the aproximated density functions
limit = 4000;
numBins = 20;
dom = -limit : limit;
figure;
% Compute histograms
[hist_A binCenters_A] = hist(x_A,numBins);
[hist_C binCenters_C] = hist(x_C,numBins);
% Normalize histograms
hist_A = hist_A/(sum(hist_A)*(binCenters_A(2)-binCenters_A(1)));
hist_C = hist_C/(sum(hist_C)*(binCenters_C(2)-binCenters_C(1)));

% Plot histograms
hold on;
bar(binCenters_A, hist_A,'y');
plot(dom',[normpdf(dom,DA.Mean,DA.Sigma)'],'b','linewidth',1);
plot(dom',[normpdf(dom,DA200.Mean,DA200.Sigma)'],'g','linewidth',1);
plot(dom',[normpdf(dom,DA20.Mean,DA20.Sigma)'],'r','linewidth',1);
legend('histogram', 'tr\_2000','tr\_200', 'tr\_20');
grid on, title 'Densities functions class A', hold off;
saveas(gcf,'mle_estimatesA.png');

figure;
hold on;
bar(binCenters_C, hist_C,'y');
plot(dom',[normpdf(dom,DC.Mean,DC.Sigma)'],'b','linewidth',1);
plot(dom',[normpdf(dom,DC200.Mean,DC200.Sigma)'],'g','linewidth',1);
plot(dom',[normpdf(dom,DC20.Mean,DC20.Sigma)'],'r','linewidth',1);
legend('histogram', 'tr\_2000','tr\_200', 'tr\_20');
grid on, title 'Densities functions class C', hold off;
saveas(gcf,'mle_estimatesC.png');

%% Estimating the optimal bayesian strategy
% 2000
q = find_strategy_2normal(DA,DC);
% Computing features vectors (test data)
xtst = compute_measurement_lr_cont(tst.images);
% Classify images
q_x = classify_2normal(tst.images, q);
% Classification error
ClassError2000 = classification_error_2normal(tst.images, tst.labels, q)
% Displaying images
figure;
show_classification(tst.images, q_x, 'AC');
saveas(gcf,'mle_classif2000.png');

% 200
q = find_strategy_2normal(DA200,DC200);
% Computing features vectors (test data)
xtst = compute_measurement_lr_cont(tst.images);
% Classify images
q_x = classify_2normal(tst.images, q);
% Classification error
ClassError200 = classification_error_2normal(tst.images, tst.labels, q)
% Displaying images
figure;
show_classification(tst.images, q_x, 'AC');
saveas(gcf,'mle_classif200.png');

% 20
q = find_strategy_2normal(DA20,DC20);
% Computing features vectors (test data)
xtst = compute_measurement_lr_cont(tst.images);
% Classify images
q_x = classify_2normal(tst.images, q);
% Classification error
ClassError20 = classification_error_2normal(tst.images, tst.labels, q)
% Displaying images
figure;
show_classification(tst.images, q_x, 'AC');
saveas(gcf,'mle_classif20.png');
