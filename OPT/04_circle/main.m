
opts.iterN = 10; % change as you like 
opts.VERBOSE = 1; % enable interactive plots 
opts.mu = 1; % for LM 

% these points are for demonstative purposes only. 
% use your own, try different configurations. 
% load my_points_1.mat 
a = [-0.7   0.7 0.7  -0.7
     -0.3  -0.3 0.8   0.3]; 

x0 = [0.0 0.2, .7]'; 

method = 'GN'; 
[x, f_history] = fit_circle(x0, a, method, opts); 
