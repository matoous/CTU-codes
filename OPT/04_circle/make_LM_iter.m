function [x_new, success] = make_LM_iter(x, a, mu)
% function [x_new, success] = make_LM_iter(x, a, mu)
% 
% makes the Levenberg-Marquardt iteration. 
%
% INPUT:
% x, a   are as usual (see dist.m for explanation) 
% mu is the damping factor (the factor which multiplies the
% regularizing identity matrix)
%
% OUTPUT: 
% success == 1 if the iteration is successful, i.e. 
% value of criterion f is decreased after the update 
% of x. 
% success == 0 in the oposite case. 
%
% x_new is the updated x if success == 1. 
% x_new is set equal to the input x if success == 0. 
    
% discard the code from here and implement the functionality: 
% x_new is the updated x. 
N = length(a);
jacobi = zeros(N,3);
for i = 1:N
    g(i,1) = sqrt((x(1)-a(1,i))^2 + (x(2)-a(2,i))^2) - x(3);
    jacobi(i,:) = [(2*x(1) - a(1,i))/(2*((x(1) - a(1,i))^2 + (x(2) - a(2,i))^2)^(1/2)), (2*x(2) - a(2,i))/(2*((x(1) - a(1,i))^2 + (x(2) - a(2,i))^2)^(1/2)), -1];
end
x_new = x - inv(jacobi'*jacobi + mu*eye(length(x)))*jacobi'*g;
new_crit = compute_criterion(x_new, a);
old_crit = compute_criterion(x, a);
if new_crit < old_crit
    success = 1;
else
    success = 0;
    x_new = x;
end