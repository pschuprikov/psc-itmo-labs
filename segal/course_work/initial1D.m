function [c, t] = initial1D(a, b, n, T, dx)
    constants;
    pn = round(dx / (b - a) * (n - 0)); 

    c = ones(n, 1);
    c(1:pn) = linspace(0, 1, pn);
    t = gT0 * ones(n, 1);
    t(1:pn) = linspace(T, gT0, pn);
end
