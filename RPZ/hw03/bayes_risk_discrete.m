function R = bayes_risk_discrete(distribution1, distribution2, W, q, x)
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
if nargin > 4
    distribution1.Prior = x;
    distribution2.Prior = 1 -x;
end
R = sum([distribution1.Prior * distribution1.Prob .* W(1,q) distribution2.Prior * distribution2.Prob .* W(2,q)]);
