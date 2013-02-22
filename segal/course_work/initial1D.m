function [c, t] = initial1D(n, Tign)
    constants;

    c = ones(n, 1);
    c(1)= 0;
    t = gT0 * ones(n, 1);
    t(1) = Tign;
end
