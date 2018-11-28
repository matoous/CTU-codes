%% Asignment: Support Vector Machines 1

clc;
close all;
clearvars;

rand('seed', 42);

%% Init STPR Toolbox

% Change this:
run('../../../../solutions/3rdparty/stprtool/stprpath.m');

%% %% Tasks, part 1 - Soft-margin SVM ====================================

%% 1. Toy data

%% Load data

load('data_33rpz_svm_toy.mat');
y(y == 2) = -1;

%% Train Soft-margin SVM

C = 1;
[w, b, sv_idx] = my_svm(X, y, C);

%% Visualize result

model.W = w;
model.b = b;
model.fun = 'linclass';

figure(2);
ppatterns(X, y); 
pboundary(model); 
hold on; 
title(['Soft-margin SVM, C = ' num2str(C)]);
plot(X(1, sv_idx), X(2, sv_idx), 'ko', 'MarkerSize', 10);
hold off;
saveas(gcf, 'softmargin_svm_C1.png');

%% 2. OCR data -----------------------------------------------------------

%% Load data & get measurements 

load('data_33rpz_svm.mat');

[X, y, normalization] = compute_measurements_2d(trn);

%% Train Soft-margin SVM

% Crossvalidation 

Cs = [0.0001, 0.001, 0.01, 0.1, 1, 10, 100];
[itrn, itst] = crossval(size(X, 2), 10);
TstErrs = [];

for C = Cs
    TstErrs(end+1) = compute_TstErr(itrn, itst, X, y, C);
end;

[minTstErr, idx] = min(TstErrs);
Copt = Cs(idx);

% Train with optimal C
[w, b, sv_idx] = my_svm(X, y, Copt);

%% Visualize result

model.W = w;
model.b = b;
model.fun = 'linclass';

figure(3);
set(gcf, 'Position', [1 1 1000 600]);
y(y==-1)=2;
ppatterns(X, y);
pboundary(model);
title('Linear SVM');
saveas(gcf, 'ocr_svm_trn.png');

%% Classify on test data

% Get test measurements
[X, y] = compute_measurements_2d(tst, normalization);

% classify
classif = classif_lin_svm(X, model);

% compute classification error
error = ...

%% Visualize result

figure(4);
set(gcf, 'Position', [1 1 1000 600]);
y(y==-1)=2;
ppatterns(X, y); 
pboundary(model); title('Test data + classification');
saveas(gcf, 'ocr_svm_tst.png');

classif(classif==-1) = 2;
show_classification(tst.images, classif, 'AC');
saveas(gcf, 'ocr_svm_classif.png');
