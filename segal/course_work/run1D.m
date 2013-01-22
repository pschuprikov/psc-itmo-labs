function run1D(c, t, scheme)
    while true
        plot(c);
        [x, y, b] = ginput(1);
        if b == 'g'
            [c, t] = scheme(c, t);
        elseif b == 'e'
            break;
        end
    end
    close all;
end
