function show_data(data, Y)

  plot(data(1, Y== 1), data(2, Y==1), 'b.');
  hold on;
  plot(data(1, Y==-1), data(2, Y==-1), 'r.');

  return
