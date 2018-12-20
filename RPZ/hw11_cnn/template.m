% Init MatConvNet framework
MCNPath = '../matconvnet-1.0-beta23/matconvnet-1.0-beta23';
run(fullfile(MCNPath, 'matlab/vl_setupnn'))

% load data
load imdb

% normalise the data
imdb.images.data = imdb.images.data / 255;

% training and validation sets
imdb.images.set = [1 * ones(1, 59000), 2 * ones(1, 1000)];

%--------------------------------------------------------------------
%% 1. Softmax with SGD
% --------------------------------------------------------------------
delete expDir/*
clear net;

% build the network
net.layers = {};
net.layers{end+1} = struct('name', 'full', ...
                           'type', 'conv', ...
                           'weights', {{1e-2*randn(28,28,1,10,'single'), zeros(1, 10,'single')}}, ... %randn(1,10,'single')}},
                           'stride', 1, ...
                           'pad', 0);
net.layers{end+1} = struct('type', 'softmaxloss');

net = vl_simplenn_tidy(net);
vl_simplenn_display(net)

% training (use "'plotStatistics', false" to suppress the graph plotting
[net, info] = cnn_train(net, imdb, @getSimpleNNBatch, 'batchSize', 1000, 'numEpochs', 100, 'expDir', 'expDir');  % plotStatistics, false);

% adding more layers + sigmoid/ReLU
delete expDir/*
clear net
net.layers = {};
net.layers{end+1} = TODO


%--------------------------------------------------------------------
%% 2. CNNs
% --------------------------------------------------------------------

clear net;
delete expDir/*

net.layers = {} ;
net.layers{end+1} = struct('name', 'conv1', ...
                           'type', 'conv', ...
                           'weights', {{1e-2*randn(3,3,1,10,'single'), zeros(1, 10,'single')}}, ...
                           'stride', 2, ...
                           'pad', 1) ;
net.layers{end+1} = struct('name', 'relu1', ...
                           'type', 'relu') ;
net.layers{end+1} = struct('name', 'full', ...
                           'type', 'conv', ...
                           'weights', {{1e-2*randn(14,14,10,10,'single'), zeros(1, 10,'single')}}, ...
                           'stride', 1, ...
                           'pad', 0);
net.layers{end+1} = struct('type', 'softmaxloss') ;

net = vl_simplenn_tidy(net);
vl_simplenn_display(net)

[net, info] = cnn_train(net, imdb, @getSimpleNNBatch, 'batchSize', 1000, 'numEpochs', 100, 'expDir', 'expDir');   %'plotStatistics', false);


% adding layers, dropout and max-pooling
clear net;
delete expDir/*

net.layers = {} ;
net.layers{end+1} = TODO

% dropout layer
net.layers{end+1} = struct('type', 'dropout', 'rate', 0.5) ;
% max-pooling layer
net.layers{end+1} = struct('type', 'pool', ...
                           'method', 'max', ...
                           'pool', [2 2], ...
                           'stride', 2, ...
                           'pad', 0) ;


% save your best network
net.layers{end}.type = 'softmax';
save('my_cnn.mat', 'net');
