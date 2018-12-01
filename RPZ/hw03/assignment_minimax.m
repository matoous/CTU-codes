load rpz_13_minimax

% initialize stpr toolbox 
run('/home/matouus/statistical-pattern-recognition-toolbox-master/stprpath.m')

% Copy needed files from previous assignment to this folder. 
% Adding path to the previous assignment is not sufficient. Upload system
% requires your code to be self contained.

% D1 priors
p_1 = 0:0.01:1;

%% Discrete

risk = zeros(size(p_1));
% fill your initials
D1 = discrete.M;
D2 = discrete.D;
letters = 'MD';

% zero one cost function (each error is penalised equally independent of the class)
W = [0 1; 1 0];

 for x = 1:length(p_1)
     D1.Prior = p_1(x);
     D2.Prior = 1 - p_1(x);
     q = find_strategy_discrete(D1,D2,W);
     risk(x) = bayes_risk_discrete(D1,D2,W,q);
 end

%
figure(1); clf(1);
plot(p_1,risk), grid, hold on;

%

D1.Prior = 0.25;
D2.Prior = 1 - D1.Prior;

risk_fix = risk_fix_q_discrete(D1, D2, p_1, find_strategy_discrete(D1,D2,W));

plot(p_1,risk_fix, 'k');

%

worst_risk = worst_risk_discrete(D1, D2, p_1);

plot(p_1,worst_risk,'r');

%ylim([0 0.2])
ylabel('R')
xlabel('p_K(x)')
legend('risk of Bayesian strategy', 'risk of fixed strategy', ...
       'worst risk of Bayesian strategy')
title('Bayesian risk for varying prior')
print('plots_discrete.png','-dpng')

% --

[q_minimax_discrete risk_minimax_discrete] = minmax_strategy_discrete(D1, D2);

[images_test_set labels_test_set] = create_test_set(images_test, letters);

labels_estimated_discrete = classify_discrete(images_test_set, q_minimax_discrete);

figure(2); clf(2);
suptitle('Classification results for minimax, discrete measurements')
show_classification(images_test_set, labels_estimated_discrete, letters)
print('minmax_classif_discrete.png','-dpng')

error_discrete = classification_error_discrete(images_test_set, labels_test_set, q_minimax_discrete)

%%  continuous

risk = zeros(size(p_1));
% fill your initials
D1 = cont.M;
D2 = cont.D;

for x = 1:length(p_1)
    D1.Prior = p_1(x);
    D2.Prior = 1 - D1.Prior;
    q = find_strategy_2normal(D1,D2);
    risk(x) = bayes_risk_2normal(D1,D2,q);
end

figure(3); clf(3);
plot(p_1,risk), grid, hold on;


D1.Prior = 0.25;
D2.Prior = 1 - D1.Prior;

risk_fix = risk_fix_q_cont(D1, D2, p_1, find_strategy_2normal(D1,D2));

plot(p_1,risk_fix, 'k');

%

worst_risk = worst_risk_cont(D1, D2, p_1);

plot(p_1,worst_risk,'r');
% ylim([0 0.1])
ylabel('R')
xlabel('p_K(x)')
legend('risk of Bayesian strategy', 'risk of fixed strategy', ...
       'worst risk of Bayesian strategy')
title('Bayesian risk for varying prior')
print('plots_cont.png','-dpng')

[q_minimax_cont risk_minimax_cont] = minmax_strategy_cont(D1, D2);

[images_test_set labels_test_set] = create_test_set(images_test, letters);

labels_estimated_cont = classify_2normal(images_test_set, q_minimax_cont);

figure(4); clf(4);
show_classification(images_test_set, labels_estimated_cont, letters)
suptitle('Classification results for minimax, continuous measurements')
print('minmax_classif_cont.png','-dpng')

error_cont = classification_error_2normal(images_test_set, labels_test_set, q_minimax_cont)

