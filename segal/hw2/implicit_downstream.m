function vs = implicit_downstream(uu, ae)
    vs = [1 - uu + 2*ae,  -ae, uu - ae ; 
                      0,   -1,       1];
end
