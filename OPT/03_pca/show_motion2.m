% vyber soubor
switch 1
    case 1
        A = load('makarena1.txt','-ASCII');
    case 2
        A = load('walk1.txt','-ASCII');
end

%% Zde doplnte Vas kod, ktery najde aproximaci A_approx %%%%%%%%%%%%

% matrix
m = mean(A);
% move the points so the minimal distance line goes through 0:0
Acent = A - m;
% SVD decomposition
[U,S,V] = svd(Acent);

% WALK 2D
R = 1;
Y = V(:,1:R);
A_approx = Acent*Y*Y' + m;


%% konec Vaseho kodu %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% definice bodu, ktere se maji spojit
conn = load('connected_points.txt');

nMarkers = size(A,2)/3;
nFrames = size(A,1);

% najdi rozsah souradnic pro vsechny body
max_x = max(max(A(:,1:3:end)));
max_y = max(max(A(:,2:3:end)));
max_z = max(max(A(:,3:3:end)));
min_x = min(min(A(:,1:3:end)));
min_y = min(min(A(:,2:3:end)));
min_z = min(min(A(:,3:3:end)));

figure; axis([min_x max_x min_y max_y min_z max_z]);
hold on;
grid on;
axis equal;

% zobraz prvni snimek
a = reshape(A(1,:), 3, nMarkers);
a(:, end+1) = NaN;
H1=plot3(a(1,conn), a(2,conn), a(3,conn), '-b.','markersize',20);

a = reshape(A_approx(1,:), 3, nMarkers);
a(:, end+1) = NaN;
H2=plot3(a(1,conn), a(2,conn), a(3,conn), '-r.','markersize',20);

% zobray postupne dalsi snimky
for i=1:size(A,1)

    a = reshape(A(i,:), 3, nMarkers);
    a(:, end+1) = NaN;
    set(H1,'XData',a(1,conn),'YData',a(2,conn),'ZData',a(3,conn));

    a = reshape(A_approx(i,:), 3, nMarkers);
    a(:, end+1) = NaN;
    set(H2,'XData',a(1,conn),'YData',a(2,conn),'ZData',a(3,conn));
    
    pause(0.001);
    drawnow;
end
