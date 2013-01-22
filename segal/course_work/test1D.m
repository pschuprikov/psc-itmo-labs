constants;

Tign = gTm;
a = 0;
b = 1e-2;
n = 10000;
dt = 0.01;

[c0, t0] = initial1D(n, Tign);
[C, T] = operators1D(a, b, n, dt);

cond(C)
cond(T)
pause;
run1D(c0, t0, @(c, t) step1D(C, T, c, t, dt, 1));
