function [x_new] = make_GN_iter(x, a)
% function [x_new] = make_GN_iter(x, a)
%
% makes the Gauss-Newton iteration. 
%
% INPUT:
% x, a   are as usual (see dist.m for explanation) 
%
% x_new is the updated x. 
N = length(a);
syms('x0', 'y0', 'r') 
%naplneni matic rovnicemi
for i = 1:N
    g(i,1) = sqrt((x0-a(1,i))^2 + (y0-a(2,i))^2) - r;
    jacobi_eq(i,:) = jacobian(g(i,1), [x0, y0, r]);
end
x0 = x(1);
y0 = x(2);
r = x(3);
jacobi = eval(jacobi_eq);
x_new = x - inv(jacobi'*jacobi)*jacobi'*eval(g);
end
