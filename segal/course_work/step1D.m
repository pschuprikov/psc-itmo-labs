function [CS, TS] = step1D(C, T, cp, tp, dt, steps, inner_steps)
    constants;

    n = size(C, 1);
    CS = zeros(n, steps);
    TS = zeros(n, steps);

    CS(:,1) = cp;
    TS(:,1) = tp;

    for i = 2:steps
        w = getW(CS(:,i-1), TS(:,i-1));
        for j = 1:inner_steps
            CS(:,i) = C \ (CS(:,i-1) + dt * gQ / gC * w);
            TS(:,i) = T \ (TS(:,i-1) - dt * w);
            w = getW(CS(:,i), TS(:,i));
        end
    end
end
