function [strong_class, wc_error, upper_bound] = adaboost(X, y, num_steps)
% [strong_class, wc_error, upper_bound] = adaboost(X, y, num_steps)
%
% Trains an AdaBoost classifier
%
%   Parameters:
%       X [K x N] - training samples, KxN matrix, where K is the number of 
%            weak classifiers and N the number of data points
%
%       y [1 x N] - training samples labels (1xN vector, contains -1 or 1)
%
%       num_steps - number of iterations
%
%   Returns:
%       strong_class - a structure containing the found strong classifier
%           .wc [1 x num_steps] - an array containing the weak classifiers
%                  (their structure is described in findbestweak())
%           .alpha [1 x num_steps] - weak classifier coefficients
%
%       wc_error [1 x num_steps] - error of the best weak classifier in
%             each iteration
%
%       upper_bound [ 1 x num_steps] - upper bound on the training error
%

%% initialisation
N = length(y);

% prepare empty arrays for results
strong_class.wc = [];
strong_class.alpha = zeros(1, num_steps);

% reduce the effect of an unbalanced training set
D = zeros(1, N);
D(y == 1) = 0.5 / length(y(y==1));
D(y == -1) = 0.5 / length(y(y==-1)); 

% error of best weak classifier on each iteration
wc_error = zeros(1,num_steps);
upper_bound = zeros(1,num_steps);
last_nonzero_err = 1;

%% AdaBoost

for t = 1:num_steps
    disp(['Step ' num2str(t)]);
    
    % select weak classifier with lowest weighted error
    [wc, err] = findbestweak(X, y, D);
    
    % stop if err >= 0.5
    if err >= 0.5
        disp(['Stop, err(>=0.5): ' num2str(err)]);
        break;
    end
    
    % add error to history
    wc_error(t) = err;
    
    % update weak classifier
    strong_class.wc(t).idx = wc.idx;
    strong_class.wc(t).theta = wc.theta;
    strong_class.wc(t).parity = wc.parity;

    % update weak classifier coefficients
    strong_class.alpha(t) = (1/2)*log((1 - err)/ err);
    
    % select weakest classifier
    Xtmp = X(wc.idx, :);
    for i = 1:N
        D(i) = D(i)*exp(-strong_class.alpha(t)*y(i)*sign(wc.parity*(Xtmp(i)-wc.theta)));
    end
    
    upper_bound(t) = sum(D);
    D = D/sum(D);
    
    EX=compute_error(strong_class,X,y);
    if EX(end) ~= 0
        last_nonzero_err = t;
    end
end

disp(['Reached zero error at step: ' num2str(EX(last_nonzero_err))]);


% make cummulative product of upper bounds
upper_bound = cumprod(upper_bound);
end
