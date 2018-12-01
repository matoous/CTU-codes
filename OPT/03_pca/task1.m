% load data
data = load('data_A.mat');

% matrix
A = data.A;
m = mean(A)

% move the points so the minimal distance line goes through 0:0
Acent = A - m;

% SVD decomposition
[U,S,V] = svd(Acent);

Y = V(:,1)
V(1,1:2)
V(1:2,1:2)
Z = [-V(1,2), V(1,1)]
m
Z * transpose(m)
transpose(m)
err = sum(sum(S(2:end).^2))
projection = Acent*Y*Y' + m;

figure
scatter(A(:,1),A(:,2),[],[0 0 1],'filled');
hold on;
scatter(projection(:,1),projection(:,2),[],'x');
xaxis = -1:0.1:10;
plot(xaxis,(Y(2)/Y(1))*(xaxis-m(1))+m(2),'g');
for i = 1:length(projection)
    plot([projection(i,1) A(i,1)], [projection(i,2) A(i,2)],'r')
end
axis equal
saveas(gcf, 'line.png');
 
%vysledky
%normalovy vektor primky,
vektor_x = V(:,2);
%smerovy vektor primky, vzdy plati ze y= (vektor_s(2)/vektor_s(1))*x
vektor_s = V(:,1);
%cislo alfa
alfa = m(2)*vektor_s(1) - m(1)*vektor_s(2)
%vektor y0
y0_vec = m' - (vektor_s\m')*vektor_s;