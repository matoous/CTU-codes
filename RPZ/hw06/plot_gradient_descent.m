function plot_gradient_descent(X, y, loss_function, w, wt, Et)
% plot_gradient_descent(X, y, loss_function, w, wt, Et)
%
%   Plots the progress of the gradient descent.
%
%   Parameters:
%       X - d-dimensional observations of size [d, number_of_observations]
%       y - labels of the observations of size [1, number_of_observations]
%       loss_function - pointer to a logistic loss function
%       w - weights of size [d, 1]
%       wt - progress of weights (of size [1, number_of_iterations])
%       Et - progress of logistic loss (of size [d, number_of_iterations])

figure;

% Plot loss function
%--------------------------------------------------------------------------
% Display range
minW = -10;
maxW = 10;

points = 20;
if size(X, 1) == 1
    w = linspace(minW, maxW, points);
    L = arrayfun(@(weights) loss_function(X, y, weights), w);
    plot(w, L);
elseif size(X, 1) == 2        
    [W1, W2] = meshgrid(linspace(minW, maxW, points), linspace(minW, maxW, points));
    L = zeros(size(W1, 1), size(W1, 2));
    for i = 1:size(W1, 1)
        for j = 1:size(W1, 2)
            L(i, j) = loss_function(X, y, [W1(i, j); W2(i, j)]);
        end
    end
    surf(W1, W2, L);
    view([90 90]);
    colorbar();
    title('Gradient descent');
else
    error('Only 1-d and 2-d loss functions can be visualized using this method.');
end

% Plot the gradient descent
%--------------------------------------------------------------------------
hold on;
offset = 0.05;
plot3(wt(1,:), wt(2,:), Et + offset, 'w.', 'MarkerSize', 15);

plot3(wt(1,:), wt(2,:), Et + offset, 'w-', 'MarkerSize', 15);

% Highlight the found minimum
plot3(w(1), w(2), logistic_loss(X, y, w) + offset, 'gs', 'MarkerSize', 10);

xlabel('w_0');
ylabel('w_1');
