function run1Dviewer(CS, TS, step)
    constants;
    total_steps = size(CS, 2);

    cur = 1;
    while true
        plot([CS(:,cur)]);
        [x, y, b] = ginput(1);
        if b == '-'
            cur = max([cur - step, 1]);
        elseif b == '+'
            cur = min([cur + step, total_steps]); 
        elseif b == 'e'
            break;
        end
    end
    close all;
end
