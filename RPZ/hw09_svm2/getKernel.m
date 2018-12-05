function [ K ] = getKernel( Xi, Xj, options )
%GETKERNEL Returns kernel matrix K(Xi, Xj)
%
% Input:
% Xi        [n x p (double)] matrix containing feature points in columns
% Xj        [n x q (double)] matrix containing feature points in columns
% options   [1 x 1 (struct)] structure with field 
%                             .kernel which specifies the type of a kernel 
%                             (rbf, linear, polynomial) and other possible parameters
%                             (like .sigma for rbf kernel or .d for polynomial)
% 
% Output: 
% K         [p x q (double)] matrix with kernel function values
% 

    if nargin < 3
        options = c2s({'kernel', 'rbf', 'sigma', 1.0});
    end

    switch options.kernel
        case 'rbf'
            n = size(Xi, 2);
            m = size(Xj, 2);
            K = zeros(n, m);
            for i = 1:n
                for j = 1:m
                    K(i,j) = exp(-(norm( Xi(:,i) - Xj(:,j) ))^2/(2*options.sigma^2));
                end
            end
        case 'polynomial'
            K = (1 + Xi'*Xj).^options.d;
        case 'linear'
            K = Xi'*Xj;
        otherwise
            K = [];
            error('getKernel: unknown kernel.');     
    end
end

