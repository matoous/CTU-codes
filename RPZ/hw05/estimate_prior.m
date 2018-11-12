function prior = estimate_prior(idLabel, labelling)
% prior = estimate_prior(idLabel, labelling)
%
%   Estimates prior probability of a class.
%
%   Parameters:
%       idLabel - id of the selected class
%       labelling - <1 x n> vector of label ids
%
%   Returns:
%       prior - prior probability
prior = sum(labelling == idLabel) / length(labelling);