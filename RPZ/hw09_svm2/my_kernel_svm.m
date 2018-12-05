function [ model ] = my_kernel_svm( X, y, C, options)
%MY_KERNEL_SVM Solves kernel soft-margin SVM dual task and outputs model
%   
% Input:
% X         [n x m (double)] matrix containing feature points in columns
% y         [1 x m (double)] vector with labels (-1, 1) for feature points in X
% C         [1 x 1 (double)] number with regularization constant C
% options   [1x1 (struct)] (optional) structure that holds options for gsmo QP solver fields .verb (verbosity of QP solver), 
%                             .tmax (maximal number of iterations, default is inf) and .kernel specifying the kernel
%                             that should be used + other fields as required by a specified kernel
% 
% Output:
% model     [1 x 1 (struct)] structure with fields .sv (support vectors), y (support vectors labels), .alpha 
%                             (lagrange multipliers), .options (structure with parameters for kernel), .b (bias term), 
%                             .fun (function that will be used for classification)
% 
    
    if nargin < 4
        options = c2s({'verb', 1, 'tmax', inf, 'kernel', 'RBF', 'sigma', 1.0});
    end
    
    N = numel(y);
    
    H = getKernel(X, X, options) .* (y' * y);
    f = -1*ones(N,1);
    a = y';
    b = 0;
    LB = zeros(N,1);
    UB = C*ones(N,1);
   
    % Solve QP (dual task of SVM)
    [alpha, fval, stat] = gsmo(H, f, a, b, LB, UB, zeros(N, 1), [], options);
    
    % Check the exit flag 
    if (~stat.exitflag)
        model = [];
        warning('my_kernel_svm: Exitflag not set. Solution not found.'); 
        return;
    end
    
    sv_idx = find( (alpha > 0) & (alpha <= C) );
              
    
    % name of function which will be used for classification
    model.fun = 'classif_kernel_svm';
    % support vectors (not just its indices!)
    model.sv = X(:,sv_idx);
    % labels of support vectors
    model.y = y(sv_idx);
    % non-zero lagrange multipliers
    model.alpha = alpha( (alpha > 0) & (alpha <= C) );
    % structure with parameters of used kernel
    model.options = options;
    % bias term
    model.b = model.y(1) - (model.alpha .* model.y(:))' * getKernel(model.sv, model.sv(:, 1), options);

end

