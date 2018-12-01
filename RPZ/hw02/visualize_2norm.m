function visualize_2norm( cont1, cont2, q )
% visualize_2norm( cont1, cont2, q )
%
%   Visualize a strategy for 2 normal distributions.
%
%   Parameters:
%       cont1.Mean - normal probability distribution mean
%       cont1.Sigma - normal probability distribution standard deviation
%       cont2 - ...
%       q - strategy
%               q.t1 q.t2 - two descision thresholds 
%               q.decision - 3 decisions for intervals (-inf, t1>, (t1, t2>, (t2, inf)
%                            shape <1 x 3>

% Plotting the Gaussian Mixture Model
f = figure;
f, pgmm(cont1,{'color','r'}), hold on;
pgmm(cont2,{'color','r'});

line([q.t1 q.t1],[0 3*1.0e-4]);
line([q.t2 q.t2],[0 3*1.0e-4]);
hold off;
saveas(f, "thresholds.png")
