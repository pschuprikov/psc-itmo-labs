function w = getW(ca, t)
    constants;
    w = -gK * (ca .^ gAl) .* exp((-gE / gR) ./ t);
end
