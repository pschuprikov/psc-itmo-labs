function vs = explicit_downstream(uu, ae)
    vs = [1 + uu - 2*ae, ae, ae - uu ;
                      0, -1,       1];
end
