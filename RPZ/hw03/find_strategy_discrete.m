function q = find_strategy_discrete(distribution1, distribution2, W, x)
% q = find_strategy_discrete(distribution1, distribution2, W)
%
%   Find bayesian strategy for 2 discrete distributions.
%   
%   Parameters:
%       distribution1.Prob      pXk(x|A) given as a <1 × n> vector
%       distribution1.Prior 	prior probability pK(A)
%       distribution2.Prob      ...
%       distribution2.Prior 	...
%       W - cost function matrix, <states x decisions>
%                                (nr. of states is fixed to 2)
%
%   Return: 
%       q - optimal strategy <1 x n>

if nargin > 3
    distribution1.Prior = x;
    distribution2.Prior = 1 - x;
end
q = ones(1, size(distribution1.Prob, 2));
for n = 1:size(q, 2)
   p1 = distribution1.Prob(n) * distribution1.Prior * W(1, 1) + distribution2.Prob(n) * distribution2.Prior * W(2, 1);
   p2 = distribution1.Prob(n) * distribution1.Prior * W(1, 2) + distribution2.Prob(n) * distribution2.Prior * W(2, 2);
   if p2 < p1
       q(n) = 2;
   end
end
