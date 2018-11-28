function [ w, b, sv_idx ] = my_svm( X, y, C, options )
%MY_SVM Solves linear soft-margin SVM dual task and outputs w, b and support vectors indices
%   
% Input:
% X         [n x m (double)] matrix containing feature points in columns
% y         [1 x m (double)] vector with labels (-1, 1) for feature points in X
% C         [1 x 1 (double)] number with regularization constant C
% options   [1x1 (struct)] (optional) structure that holds options for gsmo QP solver fields .verb (verbosity of QP solver), .tmax (maximal number of iterations, default is inf)
% 
% Output:
% w         [n x 1 (double)] normal vector of the separating hyperplane
% b         [1 x 1 (double)] number, the bias term
% sv_idx    [p x 1 (double)] vector with indices of support vectors 

    % Add options field if absent on input
    if nargin < 4
        options.verb = true;
    end;

    % Prepare input for QP solver
    N = numel(y);
    % H is a matrix [N x N] and its elements are H(i, j) = y_i*y_j*x_i'*x_j
    H = ...
    f = ...
    % a is a vector [N x 1] containing class identifiers y_i
    a = ...
    b = ...
    % LB is a vector [N x 1] lower bound for constraints on alpha
    LB = ...
    % UB is a vector [N x 1] upper bound for constraints on alpha
    UB = ...
    
    % Solve QP task
    [alpha, fval, stat, Nabla] = gsmo(H, f, a, b, LB, UB, zeros(N, 1), [], options);
   
    % Check the exit flag 
    if (~stat.exitflag)
        w = nan(size(X, 1), 1);
        b = nan(1);
        sv_idx = [];
        warning('my_svm: Exitflag not set. Solution not found.'); 
        return;
    end;
    
    % Get support vector indices
    sv_idx = ...
    
    % Transform solution of dual task to primal one
    w = ...
    b = ...
        
    
end

