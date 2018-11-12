function risks = risk_fix_q_cont(D1, D2, D1_priors, q )
% risks = risk_fix_q_cont(D1, D2, D1_priors, q )
%
%   Computes risk(s) for varying prior.
%
%   Parameters:
%       D1, D2 - normal distributions, prior not needed
%       D1_priors <1xn> vector of D1 priors
%       q - strategy
%
%   Returns:
%       risks - <1xn> vector of bayesian risk of the strategy q with
%               varying prior D1_priors

W = [0 1; 1 0];
risks = arrayfun(@(x) bayes_risk_2normal(D1,D2,q, x), D1_priors);