function run1Dviewer(c, t, dostep, constants)
    constants();

    n = size(c, 1);

    while (true)
        w = -getW(c, t, constants);
        cpl = c;
        tpl = max(zeros(n, 1), min(ones(n, 1), (t - gMinT) / (gMaxT - gMinT)));
        wpl = max(zeros(n, 1), min(ones(n, 1), (w - gMinW) / (gMaxW - gMinW)));
        plot([c, tpl, wpl]);
        usleep(1000);
        for ss = 1:20
            [c, t] = dostep(c, t);
        end
    end
    close all;
end
