function [C, T] = operators1D(a, b, n, dt)
    constants;

    dz = (b - a) / (n - 1);
    col = ones(n, 1);

    dtz = gD * dt / (dz * dz)

    c = [-dtz, 1 + 2 * dtz, -dtz]
    C = spdiags(col * c, [-1, 0, 1], n, n); 
    C(1, 1:2) = [1 0];
    C(n, n) += -dtz;

    ltz = gLd * dt / (gC * gRo * dz * dz)

    t = [-ltz, (1 + 2 * ltz), -ltz]
    T = spdiags(col * t, [-1, 0, 1], n, n);
    T(1, 1:2) = [1, 0];
    T(n, n) += -ltz;
end
