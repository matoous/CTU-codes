function [data Y] = generate_data(num_pos, num_neg, ...
				  pos_mean, pos_std, neg_min_radius)

  if ~exist('pos_mean', 'var')
    pos_mean = 0;
  end
  if ~exist('pos_std', 'var')
    pos_std = 0.1;
  end
  if ~exist('neg_min_radius', 'var')
    neg_min_radius = 0.3;
  end
  
  % positive
  data_pos = [pos_mean + pos_std * randn(1, num_pos); ...
	      pos_mean + pos_std * randn(1, num_pos)];
  
  % negative
  data_neg = [];
  data_length = 0;
  while data_length < num_neg
    new_data = [1 - 2*rand(1, num_neg - data_length); ...
		1 - 2*rand(1, num_neg - data_length)];
    dist = sqrt(sum(new_data.^2));
    idxs = find(dist > neg_min_radius & dist < 1);
    data_neg = [data_neg new_data(:, idxs)];
    data_length = length(data_neg);
  end

  data = [data_pos data_neg];
  Y = [ones(1, num_pos), -1*ones(1, num_neg)];

  return
