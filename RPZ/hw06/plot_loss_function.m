function plot_loss_function(X, y, loss_function)
    minW = -3;
    maxW = 3;
    points = 20;
    if size(X, 1) == 1
        w = linspace(minW, maxW, points);
        L = arrayfun( @(weights) loss_function(X, y, weights), w);
        plot(w, L);
    elseif size(X, 1) == 2        
        [W1,W2] = meshgrid(linspace(minW, maxW, points),linspace(minW, maxW, points));
        L = zeros(size(W1, 1), size(W1,2));
        for i = 1:size(W1,1)
            for j = 1:size(W1,2)
                L(i,j) = loss_function(X,y, [W1(i,j);W2(i,j)]);
            end
        end
        surf(W1,W2,L);
    else
        error('Only 1-d and 2-d loss functions can be visualized using this method.');
    end
end