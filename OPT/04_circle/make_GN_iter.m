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
    g(i,1) = sqrt((x0-a(1,i))^2 + (y0-a(2,i))^2) - r^2;
    gdiff(i,:) = [diff(g(i),x0) diff(g(i),y0) diff(g(i),r)];
end
x0 = x(1);
y0 = x(2);
r = x(3);
eval_gdiff = eval(gdiff);
eval_g = eval(g);
x_new = x' - inv(eval_gdiff'*eval_gdiff)*eval_gdiff'*eval_g;
x
x_new