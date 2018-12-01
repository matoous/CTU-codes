function R = bayes_risk_2normal(distribution1, distribution2, q, x)
% R = bayes_risk_2normal(distribution1, distribution2, q)
%
%   Compute bayesian risk of a strategy q for 2 normal distributions and zero-one loss function.
%
%   Parameters:
%       distribution1 - parameters of the normal dist. distribution1.Mean, distribution1.Sigma
%       distribution2 - the same as distribution1
%       q - strategy
%               q.t1 q.t2 - two descision thresholds 
%               q.decision - 3 decisions for intervals (-inf, t1>, (t1, t2>, (t2, inf)
%                            shape <1 x 3>
%
%   Return:
%       R - bayesian risk, scalar
if nargin > 3
    distribution1.Prior = x;
    distribution2.Prior = 1 -x;
end

first = [-Inf q.t1];
second = [q.t1 q.t2];
third = [q.t2 Inf];
sigma_vec = [distribution1.Sigma, distribution2.Sigma];
mean_vec = [distribution1.Mean, distribution2.Mean];
prior_vec = [distribution1.Prior, distribution2.Prior];
q3 = q.decision(3); q2 = q.decision(2); q1 = q.decision(1);
 
 integr_vec = [ normcdf(first,mean_vec(q1),sigma_vec(q1)),...
                normcdf(second,mean_vec(q2),sigma_vec(q2)),...
                normcdf(third,mean_vec(q3),sigma_vec(q3))];
 
 integr1 = integr_vec(2) - integr_vec(1);
 integr2 = integr_vec(4) - integr_vec(3);
 integr3 = integr_vec(6) - integr_vec(5);
 sum = prior_vec(q1)*integr1 + prior_vec(q2)*integr2 + prior_vec(q3)*integr3;
 R = 1-sum;
  