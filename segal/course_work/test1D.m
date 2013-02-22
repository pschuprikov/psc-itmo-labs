constants;

Tign = 650;
a = 0;
b = 0.01;
n = 100;
dt = 0.1;
dz = (b - a) / (n - 1);

[c0, t0] = initial1D(n, Tign);

run1Dviewer(c0, t0, @(c, t) step1D(a, b, c, t, dt, Tign, dz, 0.01));
