%% Example Title
% Summary of example objective

%% Section 1 Title
% Description of first code block
load("test_data.mat");

for i = 1:1%size(Qs)
    for u = 1:1%size(as)
        Q = Qs{i}; a = as{u};
        r = point_quadric(Q,a)
    end
end
