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
x(1:2) = x(1:2) + .1*randn(2, 1); 
x_new = x; 
success = 1;
