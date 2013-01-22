global gK  =      0; % 1600000; % 1 / sec
global gE  =           8e4; % J / mol
global gR  =         8.314; % J / mol * kg
global gQ  =           7e5; % J / kg
global gC  =          1980; % J / kg * K
global gLd =          0.13; % J / m * c * K
global gRo =           830; % kg / m^3
global gT0 =           298; % K

global gKa = gLd / (gRo * gC);

global gD  =           gKa; % m ^ 2 / sec
global gTm = gT0 + gQ / gC; % K
global gAl =             1; % 

