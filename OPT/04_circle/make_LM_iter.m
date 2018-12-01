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
N = length(a);
syms('x0', 'y0', 'r') 
for i = 1:N
    g(i,1) = sqrt((x0-a(1,i))^2 + (y0-a(2,i))^2) - r;
    jacobi_eq(i,:) = jacobian(g(i,1), [x0, y0, r]);
end
x0 = x(1);
y0 = x(2);
r = x(3);
jacobi = eval(jacobi_eq);
x_new = x - inv(jacobi'*jacobi + mu*eye(length(x)))*jacobi'*eval(g);
new_crit = compute_criterion(x_new, a);
old_crit = compute_criterion(x, a);
if new_crit < old_crit
    success = 1;
else
    success = 0;
    x_new = x;
end