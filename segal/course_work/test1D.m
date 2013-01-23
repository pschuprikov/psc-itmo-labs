constants;

Tign = 600;
a = 0;
b = 0.02;
n = 200;
dt = 0.1;

[c0, t0] = initial1D(a, b, n, Tign, 0.001);
[C, T] = operators1D(a, b, n, dt);
[CS, TS] = step1D(C, T, c0, t0, dt, 5000, 1);

run1Dviewer(CS, TS, 10);
