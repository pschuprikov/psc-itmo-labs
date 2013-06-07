function xs = implicit(A, x0, step)
    xs = x0;
    for i = 1:step
        xs = A \ xs;
    end
end;
