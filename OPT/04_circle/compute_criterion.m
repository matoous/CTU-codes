function [f] = compute_criterion(x, a)
% function [f] = compute_criterion(x, a)
% 
% compute the optimization criterion f, given parameters of
% the circle x and points a. 
f = sum(dist(x, a).^2); 
