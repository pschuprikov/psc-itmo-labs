function run1Dviewer(c, t, dostep, constants)
    constants;

    while (true)
        w = getW(c, t);
        plot([c, (t - gT0) / (800 - gT0), -w / 0.5]);
        usleep(1000);
        for ss = 1:20
            [c, t] = dostep(c, t);
        end
    end
    close all;
end
