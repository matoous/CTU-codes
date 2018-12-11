function [wc wc_error] = findbestweak(X, y, D)
% [wc wc_error] = findbestweak(X, y, D)
%
% Searches over all weak classifiers and their parametrisations (threshold
% and parity) for the weak classifier with lowest weighted classification
% error.
%
%   Parameters:
%       X [K x N] - training samples, KxN matrix, where K is the number of 
%            weak classifiers and N the number of data points
%
%       y [1 x N] - training samples labels (1xN vector, contains -1 or 1)
%
%       D [1 x N] - training sample weights
%
%   Returns:
%       wc - a structure containing the found weak classifier
%         .idx - index of the selected weak classifier
%         .theta - the classification threshold
%         .parity - the classification parity
%            The weak classifier realises following classification function
%               sign(parity * (x - theta))
%
%       wc_error - the weighted error of the selected weak classifier
%

  numwc = size(X, 1);   % number of weak classifiers
  N = size(X, 2);   % number of data samples

  best_err = Inf;
  wc_error = Inf;
  % find the weak classifier's parameters with lowest weighted error
  for I = 1:numwc
      weak_X = X(I, :); % weak classifier evaluated on all data

      % find all thresholds between different data samples
      thresh = unique(weak_X);
      if (length(thresh) > 1)
          thresh = (thresh(1:end-1) + thresh(2:end))/2;
      else
          % special case...
          thresh = [+1 -1] + thresh(1);
      end
      
      % vektorised search for the best parameters - threshold and parity
      K = length(thresh);
      classif = sign(repmat(weak_X, K, 1) - repmat(thresh', 1, N))';
      tmp_y = repmat(y', 1, K);
      tmp_D = repmat(D', 1, K);
      
      err1 = sum(tmp_D .* (classif ~= tmp_y));
      err2 = sum(tmp_D .* (-classif ~= tmp_y));
      
      [minerr1 idx1] = min(err1);
      [minerr2 idx2] = min(err2);
      
      [err parity] = min([minerr1(1), minerr2(1)]);
      if parity == 2
          parity = -1;
          theta = thresh(idx2(1));
      else
          theta = thresh(idx1(1));
      end
      
      % test if a better solution was found
      if err < best_err
          wc.idx = I;
          wc.theta = theta;
          wc.parity = parity;
          wc_error = err;
          best_err = err;
      end
  end
