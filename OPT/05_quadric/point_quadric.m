function x = point_quadric(Q,a)
    [V,D] = eig(Q,'matrix');
    N = length(Q);
    l = diag(D);
    x = zeros(1, N+1);
    x(1) = 1;
    x = x';
    for i = 1:50
        x = make_GN_iter(x, a, l)
    end
    
    % does it work?
    % transform the result back
    x = V*x(1:end-1)
    % check that it sums to one
    W = x'*Q*x
    % check the distance from point a
    d = norm(x-a)
end
