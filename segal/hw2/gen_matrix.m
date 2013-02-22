function A = gen_matrix(a, b, n, dt, mu, kappa, func)
    dx = (b - a) / (n - 1);
    
    ae = kappa * dt / (dx * dx);
    uu = mu * dt / dx;

    params = func(uu, ae);

    A = spdiags(ones(n, 1) * params(1,:), params(2,:), n, n);
end
