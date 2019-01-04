function samples = gen_kmeanspp_data()
% GEN_DATA generates data with 4 normally distributed clusters
%
% Output:
%   samples - [2 x n*4] 2 dimensional samples with n samples per cluster

mu = [[-5, 0]; [5, 0]; [0, -5]; [0, 5]];
n = 80;

samples = normrnd(repmat(mu, n, 1)', 1);

