function [w, wt, Et] = logistic_loss_gradient_descent(X, y, w_init, epsilon)
% [w, wt, Et] = logistic_loss_gradient_descent(X, y, w_init, epsilon)
%
%   Performs gradient descent optimization of the logistic loss function.
%
%   Parameters:
%       X - d-dimensional observations of size [d, number_of_observations]
%       y - labels of the observations of size [1, number_of_observations]
%       w_init - initial weights of size [d, 1]
%       epsilon - parameter of termination condition: norm(w_new - w_prev) <= epsilon
%
%   Return:
%       w - resulting weights
%       wt - progress of weights (of size [d, number_of_iterations])
%      Et - progress of logistic loss (of size [1, number_of_iterations])
w_new = w_init;
w_prev = 0;
stepsize = 0.1;
E = logistic_loss(X,y,w_init);
g = logistic_loss_gradient(X,y,w_init);
wt(:,1) = w_init;
Et(1) =E;
i=2;
while norm(w_new-w_prev)>epsilon    
    tmp = w_new-stepsize*g;
    Enew = logistic_loss(X,y,tmp);
    gnew = logistic_loss_gradient(X,y,tmp);  
    if Enew < E
        w_prev = w_new;
        w_new = tmp;
        g = gnew;
        E = Enew;
        wt(:,i) = w_new;
        Et(i) = E;
        stepsize = stepsize*2;
        i = i+1;
    else
       stepsize = stepsize/2; 
    end
end
w =w_new;