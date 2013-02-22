function run_viewer(scheme, viewer_title)
    step = 0;
    while true
        plot(scheme(step));
        title(viewer_title);
        [x, y, b] = ginput(1);
        if b == '+'
            step += 1;
        elseif b == '-'
            step = max([0 step - 1]);
        elseif b == 'e'
            break;
        end
    end
    close all;
end
