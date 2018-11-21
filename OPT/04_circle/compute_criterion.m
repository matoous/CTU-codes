function [f] = compute_criterion(x, a)
% function [f] = compute_criterion(x, a)
% 
% compute the optimization criterion f, given parameters of
% the circle x and points a. 

% discard the code from here and implement the functionality: 
f = sum(dist(x, a).^2); 
