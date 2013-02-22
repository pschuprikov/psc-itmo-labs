function [c, t] = step1D(a, b, cp, tp, dt, Tign, dz, maxdev, constants)
    constants();

    n = size(cp, 1);

    wp = getW(cp, tp, constants);
    w = wp;

    do 
        [C, T] = operators1D(a, b, n, dt, constants);
            
        cb = cp + dt * w;
        c = C \ cb;

        tb = tp - dt * gQ / gC * w;
        ltz = gLd * dt / (gC * gRo * dz * dz);
        tb(1) += Tign * ltz;
        tb(n) += gT0 * ltz;

        t = T \ tb;

        w = getW(c, t, constants);

        devc = max(max(abs(c - cp)' / max(c', ones(1, n))));
        devt = max(max(abs(t - tp)' / max(t', ones(1, n))));
        devw = max(max(abs(w - wp)' / max(w', ones(1, n))));
        
        dev = max([devc, devt, devw]);
        dt *= min([1, maxdev / dev]);
    until (maxdev > dev)
end
