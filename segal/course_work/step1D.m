function [c, t] = step1D(C, T, cp, tp, dt, steps)
    constants;

    n = size(C, 1);

    c = cp;
    t = tp;

    for i = 1:steps
        w = getW(cp, tp);
        c = C \ (t + dt * gQ / gC * w);
        t = T \ (c - dt * w);
    end
end
