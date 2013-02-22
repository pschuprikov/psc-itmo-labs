gK  =    1000000000; % 1 / sec
gE  =        110000; % J / mol
gR  =         8.314; % J / mol * kg
gQ  =           7e5; % J / kg
gC  =          1980; % J / kg * K
gLd =          0.13; % J / m * c * K
gRo =           830; % kg / m^3
gT0 =           298; % K

gKa = gLd / (gRo * gC);

gD  =        6.e-11; %gKa; % m ^ 2 / sec
gTm = gT0 + gQ / gC; % K
gAl =             1; % 

