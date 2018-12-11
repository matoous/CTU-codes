function showclassif(classif)
  
  for J=1:length(classif)
    w = classif(J).weak.thresh * classif(J).weak.feature;
    
    wlen = sqrt(sum(w.^2));
    w_perp_norm = [-w(2)/wlen, w(1)/wlen];
    
    visX = [w(1) + 2 * w_perp_norm(1), ...
	    w(1) - 2 * w_perp_norm(1)];
    visY = [w(2) + 2 * w_perp_norm(2), ...
	    w(2) - 2 * w_perp_norm(2)];
    plot(visX, visY, 'k'); %, 'LineWidth', 3);
  end
