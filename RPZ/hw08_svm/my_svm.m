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
    end

    % Prepare input for QP solver
    N = numel(y);
    
    % H is a matrix [N x N] and its elements are H(i, j) = y_i*y_j*x_i'*x_j
    H = zeros(N,N);
    for i = 1:N
        for j=1:N
            H(i,j) = y(i)*y(j)*X(:,i)'*X(:,j);
        end
    end

    f = -1*ones(N,1);
    % a is a vector [N x 1] containing class identifiers y_i
    a = y';
    b = 0;
    % LB is a vector [N x 1] lower bound for constraints on alpha
    LB = zeros(N,1);
    % UB is a vector [N x 1] upper bound for constraints on alpha
    UB = C*ones(N,1);
    
    % Solve QP task
    [alpha, ~, stat, ~] = gsmo(H, f, a, b, LB, UB, zeros(N, 1), [], options);
   
    % Check the exit flag 
    if (~stat.exitflag)
        w = nan(size(X, 1), 1);
        b = nan(1);
        sv_idx = [];
        warning('my_svm: Exitflag not set. Solution not found.'); 
        return;
    end
    
    % Get support vector indices
    sv_idx = find(alpha ~= 0);
    
    % Transform solution of dual task to primal one
    w = 0;
    for i = 1:N
       w = w + alpha(i)*y(i)*X(:,i);
    end
    b = mean(arrayfun(@(x) y(x) - w'*X(:,x), sv_idx));
end

