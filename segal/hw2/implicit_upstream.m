function vs = implicit_upstream(uu, ae)
    vs = [-ae, -uu + 2 * ae, 1 + uu - ae;
           -2,           -1,           0];
end
