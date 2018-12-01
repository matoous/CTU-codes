function [ TstErr ] = compute_TstErr( itrn, itst, X, y, C )
%COMPUTE_TSTERR Computes mean risk computed over crossvalidation folds (train svm on train set and evaluate on test set)
%
% Input:
% itrn      [1 x f (cell)] cell with f folds for crossvalidation (indices to X of training data)
% itst      [1 x f (cell)] cell with f folds for crossvalidation (indices to X of test data)
% X         [n x m (double)] matrix containing feature points in columns
% y         [1 x m (double)] vector with labels (-1, 1) for feature points in X
% C         [1 x 1 (double)] number with regularization constant C
% 
% Output:
% TstErr    [1 x 1 (double)] mean test error 
TstErr = [];
for i = 1:length(itrn)
    tr_set = itrn{i};
    ts_set = itst{i};
    [ w, b, sv_idx ] = my_svm(X(:,tr_set),y(tr_set),C);
    model.W = w; model.b = b; model.fun = "linclass";
    classif = classif_lin_svm(X(:,ts_set), model);
    TstErr(i) = sum(classif~=y(ts_set));
end
TstErr = mean(TstErr);
end

