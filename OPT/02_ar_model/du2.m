
%% Use of ar_fit_model:
[y, Fs] = audioread('gong.wav');
p = 300; 
[a, M, b] = ar_fit_model(y, p); 

% display the result 
fprintf('a0 = %f\n', a(1)); 
h = figure();
plot(a(2:end)); 
xlabel('coefficients {\it a_1, a_2, ... a_p} of the AR model')

%% Use of ar_predict:
N = length(y); 
y0 = y(1:p); 
[y_pred] = ar_predict(a, y0, N); 

% show predicted signal (truncated)
h = figure();
K = 10000; 
plot(y(1:K), 'b'); hold on 
plot(y_pred(1:K), 'r'); hold off 
legend('original', 'AR model'); 
xlabel('time')
ylabel('y')
saveas(h, 'graph.png')

fprintf('Mean absolute deviation   : %f\n', mean(abs(y - y_pred)) );
fprintf('Root-mean-square deviation: %f\n', sqrt( mean( (y - y_pred)).^2 ));

%% solve_ls
%
a2 = solve_ls(M, b); 
% now compare a, a2 

