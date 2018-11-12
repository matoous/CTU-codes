function [q risk] = minmax_strategy_cont(D1, D2)
% [q risk] = minmax_strategy_cont(D1, D2)
%
%   Find minmax strategy.
%
%   Parameters:
%       D1, D2 - 2 normal distributions
%                D1.Sigma, D1.Mean
%                prior not needed
%
%   Returns:
%       q - strategy (see find_strategy_2normal)
%       risk - bayes risk of the minimax strategy q
minn = fminbnd(@(x) worst_risk_cont(D1,D2,x),0,1);
risk = min(worst_risk_cont(D1,D2,minn));
q = find_strategy_2normal(D1,D2,minn);

