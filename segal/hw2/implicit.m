function xs = implicit(A, x0, step)
    xs = (A ^ step) \ x0;
end;
