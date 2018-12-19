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
jacobi = zeros(N,3);
for i = 1:N
    g(i,1) = sqrt((x(1)-a(1,i))^2 + (x(2)-a(2,i))^2) - x(3);
    jacobi(i,:) = [(2*x(1) - a(1,i))/(2*((x(1) - a(1,i))^2 + (x(2) - a(2,i))^2)^(1/2)), (2*x(2) - a(2,i))/(2*((x(1) - a(1,i))^2 + (x(2) - a(2,i))^2)^(1/2)), -1];
end
x_new = x - inv(jacobi'*jacobi)*jacobi'*g;
end
