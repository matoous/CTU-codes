function vis_boundary(data, Y, classif)

  cla;
  
  rangeX = [-1:0.01:1];
  rangeY = [-1:0.01:1];
  
  % decision bitmap
  [x,y] = meshgrid(rangeX, rangeY);
  bitX = [reshape(x, 1, length(x(:))); reshape(y, 1, length(y(:)))];
  bitTSLength = size(bitX, 2);
  
  % find train error in the bitmap
  result = strongeval_real(classif, bitX);
  
  bitRes = reshape(result, size(x));
  
  hue_sat = rgb2hsv([0.53 0.75 0.66]);
  hue_sat = hue_sat(2:3);
  minResp = min(bitRes(:)); %-10;
  maxResp =  max(bitRes(:)); %10;

  % generate color image
  img = zeros(size(bitRes, 1), size(bitRes, 2), 3);
  value = (bitRes - minResp) / (maxResp - minResp);
  hsv = [repmat(hue_sat, length(value(:)), 1),  value(:)];
  rgb = hsv2rgb(hsv);
  img(:, :, 1) = reshape(rgb(:, 1), size(bitRes));
  img(:, :, 2) = reshape(rgb(:, 2), size(bitRes));
  img(:, :, 3) = reshape(rgb(:, 3), size(bitRes));
  
  image(rangeX, rangeY, img), axis xy, axis off, axis equal;
  ax1 = gca;
  axis([rangeX(1) rangeX(end) rangeY(1) rangeY(end)]);
  
  hold on;
  
  % show training set
  show_data(data, Y);
  
  % show classifier
  showclassif(classif)
  
  axis off
  axis equal
  set(gca, 'XLim', [rangeX(1) rangeX(end)]);
  set(gca, 'YLim', [rangeY(1) rangeY(end)]);
  
  drawnow

  return
  