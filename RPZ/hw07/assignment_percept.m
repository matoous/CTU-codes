% Assignment: The perceptron algorithm

%% init
run('/home/matouus/statistical-pattern-recognition-toolbox-master/stprpath.m')


% Use this command to create data and save it into a .mat file (e.g. myData.mat)
createdata;


%% Task 1
% Load data from the .mat file
load('myData.mat');

%The data are now stored in the X and y variables, display them to check
%everything is loaded fine
disp(X);
disp(y);

% Run the perceptron algorithm with at most 100 iterations
[w b] = perceptron(X, y, 100);

if isnan(w)
    disp('The algorithm did not converge in given iteration limit.');
    return;
end

%Show the data and the resulting linear classifier
figure, ppatterns(X,y), hold on, title 'Perceptron algorithm';
pline(w,b);


%% Task 2
% Load data from the .mat file
load('myNonseparableData.mat');

Z = lift_dimension(X);

%The data are now stored in the X and y variables, display them to check
%everything is loaded fine
%disp(Z);
%disp(y);

% Run the perceptron algorithm with at most 100 iterations
[w b] = perceptron(Z, y, 100);

if isnan(w)
    disp('The algorithm did not converge in given iteration limit.');
    return;
end

%Show the data and the resulting classifier
figure, ppatterns(Z,y), hold on, title 'Perceptron algorithm in a lifted space';

model = struct('w',w,'b', b, 'fun','classif_quadrat_perc');
pboundary(model);
