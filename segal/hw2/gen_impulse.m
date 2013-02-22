function ts = gen_impulse(a, b, n, x0, x1, t)
    start_idx = round((x0 - a) / (b - a) * (n - 1)) + 1;
    end_idx   = round((x1 - a) / (b - a) * (n - 1)) + 1;

    ts = zeros(n, 1);
    ts(start_idx:end_idx) = t;
end
