% RPZ assigment: Logistic regression


%% Init
run('/home/matouus/statistical-pattern-recognition-toolbox-master/stprpath.m')

%% Classification of letters A and C
%--------------------------------------------------------------------------
% Load training data and compute features
load ('data_33rpz_logreg.mat');

% Prepare the training data
trainX = [compute_measurements(trn.images); ones(1,length(trn.images))];

% Training - gradient descent of the logistic loss function
% Start at a fixed point:
w_init = [-7; -8];
% or start at a random point:
% w_init = 20 * (rand(size(trainX, 1), 1) - 0.5);
epsilon = 1e-2;
[w, wt, Et] = logistic_loss_gradient_descent(trainX, trn.labels, w_init, epsilon)

% Plot the progress of the gradient descent
% plotEt
figure;
plot(1:length(Et), Et);
xlabel('Iteration');
ylabel('Loss function');
xlim([0 20]);
title('Logistic regression error');
saveas(gcf, 'E_progress_AC.png');

%
figure;
plot_gradient_descent(trainX, trn.labels, @logistic_loss, w, wt, Et);
saveas(gcf, 'w_progress_2d_AC.png')

% plot aposteriori probabilities
thr = get_threshold(w);
lbls = trn.labels;
for ii = 1:size(trainX,2)
   if lbls(ii)~=-1
      twoss(ii) = trainX(1,ii);
   else
      oness(ii) = trainX(1,ii); 
   end
end
figure;
hold on;
scatter(twoss,zeros(1,size(trainX,2)-1),'x');
scatter(oness,ones(1,size(trainX,2)),'x');
hold on;
plot(-4:0.1:4,sigmoid(-4:0.1:4,thr, 0.1),'b');
hold on;
plot(-4:0.1:4,1-sigmoid(-4:0.1:4,thr, 0.1),'r');
saveas(gcf,'aposteriori.png');

% Prepare the test data
testX = [compute_measurements(tst.images); ones(1,length(tst.images))];
    
% Classify letter test data and calculate classification error
classifiedLabels = classify_images(testX, w);

testError = sum(classifiedLabels ~= tst.labels) / length(tst.labels);
fprintf('Letter classification error: %.2f%%\n', testError * 100);

% Visualize classification results
show_classification(tst.images, classifiedLabels, 'AC');
saveas(gcf, 'classif_AC.png');


%% Classification of MNIST digits
%--------------------------------------------------------------------------
% Load training data
load('mnist_01_trn');

% prepare the training data
Xtrain = [ones(length(X),1), X];
ytrain = y;

% Training - gradient descent of the logistic loss function
w_init = rand(size(X, 1), 1);
epsilon = 1e-2;
[w, ~, Et] = logistic_loss_gradient_descent(Xtrain, ytrain, w_init, epsilon);

% Plot the progress of the gradient descent
% plotEt
figure;
plot(1:length(Et), Et);
xlabel('Iteration');
ylabel('Loss function');
title('Logistic regression error');
saveas(gcf, 'E_progress_MNIST.png');
    
% Load test data
load('mnist_01_tst');
Xtest = X;
ytest = y;

% prepare the training data

% Classify MNIST test data and calculate classification error
classifiedLabels = classify_images(Xtest, w);
testError = sum(classifiedLabels ~= ytest) / length(ytest);
fprintf('MNIST digit classification error: %.2f%%\n', testError * 100);

% Visualize classification results
show_mnist_classification(Xtest(:, :), classifiedLabels);
saveas(gcf, 'classif_MNIST.png');
