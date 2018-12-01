% load data
A = load('walk1.txt');

% matrix
m = mean(A);
% move the points so the minimal distance line goes through 0:0
Acent = A - m;
% SVD decomposition
[U,S,V] = svd(Acent);

% WALK 2D
R = 2;
Y = V(:,1:R);
projection = Acent*Y*Y' + m;
resMat = A-projection;   
distance = norm(resMat,'fro')^2;
display(distance);

%2D
figure
lin_komb = Acent*Y;
line(lin_komb(:,1),lin_komb(:,2));
title('walk\_2d');
xlabel('x');ylabel('y');
saveas(gcf, 'walk_2d.png');

% WALK 3D
R = 3;
Y = V(:,1:R);
projection = Acent*Y*Y' + m;
resMat = A-projection;   
distance = norm(resMat,'fro')^2;
%3D
figure
lin_komb = Acent*Y;
plot3(lin_komb(:,1),lin_komb(:,2)',lin_komb(:,3),'g');
title('walk\_3d');
xlabel('x');ylabel('y');zlabel('z');
saveas(gcf, 'walk_3d.png');
 
% load data
A = load('makarena1.txt');

% matrix
m = mean(A);
% move the points so the minimal distance line goes through 0:0
Acent = A - m;
% SVD decomposition
[U,S,V] = svd(Acent);

% WALK 2D
R = 2;
Y = V(:,1:R);
projection = Acent*Y*Y' + m;
resMat = A-projection;   
distance = norm(resMat,'fro')^2;
%2D
figure
lin_komb = Acent*Y;
line(lin_komb(:,1),lin_komb(:,2));
title('makarena\_2d');
xlabel('x');ylabel('y');
saveas(gcf, 'makarena_2d.png');

% WALK 3D
R = 3;
Y = V(:,1:R);
projection = Acent*Y*Y' + m;
resMat = A-projection;   
distance = norm(resMat,'fro')^2;
%3D
figure
lin_komb = Acent*Y;
plot3(lin_komb(:,1),lin_komb(:,2)',lin_komb(:,3),'g');
title('makarena\_3d');
xlabel('x');ylabel('y');zlabel('z');
saveas(gcf, 'makarena_3d.png');