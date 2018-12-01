function test_temps(x, omega, t, T)

set(0,'defaultAxesFontSize',20)
h = figure();
set(h, 'Position', [0 0 1024 768])

plot(t, T, 'ro'); hold on
[t1, t2] = deal(min(t), max(t));
t_ = linspace(t1, t2, 512);
T_ = x(1) + x(2) * t_ + x(3)*sin(omega*t_) + x(4)*cos(omega*t_);
plot(t_, T_, 'b')
xlabel('day')
ylabel('temperature')

end