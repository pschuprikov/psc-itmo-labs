function w = getW(ca, t, constants)
    constants();
    w = -gK * (ca .^ gAl) .* exp((-gE / gR) ./ t);
end
