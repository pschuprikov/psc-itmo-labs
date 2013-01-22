function u = getU()
    constants;
    u = sqrt(2 * gK * gLd / (gQ * gRo) * gR * gTm / gE * exp(-gE / (gR * gTm)));
end
