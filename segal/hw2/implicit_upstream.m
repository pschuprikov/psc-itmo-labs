function vs = implicit_upstream(uu, ae)
    vs = [-ae, -uu - ae, 1 + uu + 2 * ae;
           1,        -1,           0];
end
