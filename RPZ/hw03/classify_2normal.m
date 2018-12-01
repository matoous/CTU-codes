function label = classify_2normal(imgs, q)
% label = classify_2normal(imgs, q)
%
%   Classify images using continuous measurement and strategy q.
%
%   Parameters:
%       images - test set images, <h x w x n>
%       q - strategy
%               q.t1 q.t2 - two descision thresholds 
%               q.decision - 3 decisions for intervals (-inf, t1>, (t1, t2>, (t2, inf)
%                            shape <1 x 3>
%
%   Return:
%       label - image labels, <1 x n>
first = q.decision(1); second = q.decision(2); third = q.decision(3);
measurements_continuous = compute_measurement_lr_cont(imgs);
 
for x = 1:length(imgs)
    if measurements_continuous(x) <= q.t1
        labels(x) = first;
    elseif measurements_continuous(x) > q.t2
        labels(x) = third;
    else
        labels(x) = second;
    end
end
label = labels;