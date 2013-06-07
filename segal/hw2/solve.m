clear ; close all; clc
mu    = 0.0;
kappa = 1;

a = 0;
b = 1;

n = 1000;
dt = 0.011;

x0 = gen_impulse(a, b, n, 0, 0.3, 10);

A = gen_matrix(a, b, n, dt, mu, kappa, @explicit_upstream);
run_viewer(@(step) explicit(A, x0, step), 'explicit upstream');

A = gen_matrix(a, b, n, dt, mu, kappa, @explicit_downstream);
run_viewer(@(step) explicit(A, x0, step), 'explicit downstream');

A = gen_matrix(a, b, n, dt, mu, kappa, @implicit_upstream);
run_viewer(@(step) implicit(A, x0, step), 'implicit upstream');

A = gen_matrix(a, b, n, dt, mu, kappa, @implicit_downstream);
run_viewer(@(step) implicit(A, x0, step), 'implict downstream');
