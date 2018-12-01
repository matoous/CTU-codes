function R = bayes_risk_discrete(distribution1, distribution2, W, q)
% R = bayes_risk_discrete(distribution1, distribution2, W, q)
%
%   Compute bayesian risk for a discrete strategy q
%
%   Parameters:
%           distribution1.Prob      pXk(x|A) given as a <1 × n> vector
%           distribution1.Prior 	prior probability pK(A)
%           W                       cost function matrix
%                                   dims: <states x decisions>
%                                   (nr. of states and decisions is fixed to 2)
%           q                       strategy - <1 × n> vector, values 1 or 2
%
%   Return:
%           R - bayesian risk, <1 x 1>
R = 0;
for n = 1:size(distribution1.Prob, 2)
    R = R + distribution1.Prior * distribution1.Prob(n) * W(1, q(n)) + distribution2.Prior * distribution2.Prob(n) * W(2, q(n));
end
