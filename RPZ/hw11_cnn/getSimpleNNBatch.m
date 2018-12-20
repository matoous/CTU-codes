function [images, labels] = getSimpleNNBatch(imdb, batch)

images = imdb.images.data(:,:,:,batch) ;
labels = imdb.images.labels(1,batch) ;
