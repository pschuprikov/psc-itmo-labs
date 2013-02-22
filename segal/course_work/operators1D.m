function [C, T] = operators1D(a, b, n, dt, constants)
    constants();

    dz = (b - a) / (n - 1);
    col = ones(n, 1);

    dtz = gD * dt / (dz * dz);

    c = [1 + 2 * dtz, -dtz, -dtz];
    C = spdiags(col * c, [0, -1, 1], n, n);
    C(n, n) += -dtz;

    ltz = gKa * dt / (dz * dz);

    t = [1 + 2 * ltz, -ltz, -ltz];
    T = spdiags(col * t, [0, -1, 1], n, n);
end
