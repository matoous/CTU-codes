function thr = get_threshold(w)
% thr = get_threshold(w)
%
%   Returns the optimal decision threshold given the sigmoid parameters w
%
%   Parameters:
%       w - sigmoid parameters [2, 1]
%
%   Return:
%       thr - calculated threshold (scalar)
thr = -w(1) / w(2);