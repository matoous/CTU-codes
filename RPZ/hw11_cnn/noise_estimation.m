% load data
load imdb

imdb.images.data(:,:,1,:) = imdb.images.data(:,:,1,:) + imdb.images.data_mean;

% lets take a look at some noise in the images
X = reshape(imdb.images.data(1:4,1:4,1,59001:60000),1,16000);

white_perc = length(X(X==250))/length(X); % percentage of white grain noise

no_change_perc = length(X(X==0))/length(X); % percentage of unchange pixels

X = X(X~=0); % 0 should be the default color in this part of image at almost all the time
X = X(X~=250);

% plot histogram (right half)
figure
histogram(X); % we will se from the histogram that the color change is normal distribution

% plot full histogram 
Y = [X -X];
figure
histogram(Y);

% estimate normal distribution parameters
[muhat,sigmahat] = normfit(Y); % estimate distribution parameters

my_interval = 1:18;
sample_interval = 59201:59212;
figure 
colormap(gray)  
for i = my_interval 
    subplot(5,6,i) 
    digit = screw_image(reshape(imdb.images.data(:,:,1,i), [28,28]));
    display(digit)
    imagesc(digit)
    title(num2str(imdb.images.labels(i)))
end
for i = sample_interval
    subplot(5,6,i-59200+18)                              
    digit = reshape(imdb.images.data(:,:,1,i), [28,28]);   
    imagesc(digit)                           
    title(num2str(imdb.images.labels(i)))
end