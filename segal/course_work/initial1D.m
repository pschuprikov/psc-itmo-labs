function [c, t] = initial1D(n, T)
    constants;

    c = ones(n, 1);
    c(1) = 0;
    t = gTm * ones(n, 1);
    t(1) = T;
end
