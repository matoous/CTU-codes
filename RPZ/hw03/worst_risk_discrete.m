function worst_risks = worst_risk_discrete(D1, D2, D1_priors)
% worst_risks = worst_risk_discrete(D1, D2, D1_priors)
%
%   Compute worst possible risks of a bayesian strategies.
%
%   Parameters:
%       D1, D2 - discrete distributions
%       D1_priors - <1 x n> vector of D1 priors to be used
%
%   Returns:
%       worst_risks - <1 x n> worst risk of bayesian strategies
%                     for D1, D2 with different priors D1_priors


%   Hint: for all D1_priors calculate bayesian strategy and 
%   corresponding maximal risk.

W = [0 1; 1 0];
worst_risks = zeros(size(D1_priors));
for x = 1:length(worst_risks)
    D1.Prior = D1_priors(x);
    D2.Prior = 1 - D1.Prior;
    q = find_strategy_discrete(D1,D2,W);
    D1.Prior = 1;
    D2.Prior = 0;
    risk1 = bayes_risk_discrete(D1,D2,W,q);
    D1.Prior = 0;
    D2.Prior = 1;
    risk2 = bayes_risk_discrete(D1,D2,W,q);
    worst_risks(x) = max(risk1, risk2);
end

