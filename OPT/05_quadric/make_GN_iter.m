function [x_new] = make_GN_iter(x, a, V)
% function [x_new] = make_GN_iter(x, v)
%
% makes the Gauss-Newton iteration. 
%
% INPUT:
% x - array of params [y_1..y_n gamma]
% v - point a to which we are finding the closest point   
%
% x_new is the updated x. 
N = length(x) - 1;
y = sym('y', [1 N]);
gm = sym('gm');

% equations for individual coordinates of point x
for i = 1:N
    g(i,1) = -2*(a(i) - y(i) + x(end)*V(i)*y(i));
    jacobi_eq(i,:) = jacobian(g(i,1), y(i));
end

% equation for gamma
g(N+1,1) = V'*(y'.^2) - 1;
jacobi_eq(N+1,:) = jacobian(g(N+1,1), gm);
    
% assign variables
for i = 1:N
      eval(sprintf('y%d = x(i)', i));
end
gm = x(end);

jacobi = eval(jacobi_eq)
g = eval(g)

x_new = x - inv(jacobi'*jacobi)*jacobi'*g;

end
