constants;

Tign = gTm;
a = 0;
b = 0.1;
n = 2000;
dt = 0.1;

[c0, t0] = initial1D(a, b, n, Tign, 0.01);
[C, T] = operators1D(a, b, n, dt);
[CS, TS] = step1D(C, T, c0, t0, dt, 1000, 1);

run1Dviewer(CS, TS, 1);
