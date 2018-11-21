function [x, f_history] = fit_circle(x, a, method, opts)
%
%
if opts.VERBOSE
    clc, close all
    plot(a(1,:),a(2,:),'bx','MarkerSize',10);
    xlim([-1,1])
    ylim([-1,1])
    hold on
    axis equal
    h = plot_circle(x);
    title('Initialization')
    disp('=== Press any key ===')
    pause
end

f0 = compute_criterion(x, a); % at init
f_history = zeros(1, opts.iterN+1); 
f_history(1) = f0; 


if strcmp(method, 'GN')
else
    mu = opts.mu; 
end

for iter = 1:opts.iterN 
   if strcmp(method, 'GN')
       x = make_GN_iter(x, a);
   else % LM 
       [x, success] = make_LM_iter(x, a, mu);
       if success
           mu = mu/3; 
       else
            mu = mu*3; 
       end
   end

   f = compute_criterion(x, a) 
   f_history(iter+1) = f;

   if opts.VERBOSE 
       delete(h) 
       h = plot_circle(x); 
       title(sprintf('Iteration %i', iter))
       disp('=== Press any key ===')
       pause
   end

   % is radius >0 ?
   if x(3) <=0 
       x(3) = std(a(:)); %reinitialize
   end
   
end
hold off

end

    
function h = plot_circle(x); 
    [c, r] = deal( x(1:2), x(3));
    phi = linspace(0, 2*pi, 100); 
    h = plot(r*cos(phi)+c(1), r*sin(phi)+c(2), 'r');
end
