import geometry;
import plain;

size(10cm, 0);

point v0 = (0, 5);
point v1 = (10, 5);
point v2 = (10, 0);
point v3 = (0, 0);

point a0 = (0, 2);
point a1 = (10, 2);
point b0 = (0, 4);
point b1 = (10, 4);

fill(v0--b0--b1--v1--cycle, lightgreen);
fill(b0--a0--a1--b1--cycle, lightred);
fill(a0--v3--v2--a1--cycle, lightblue);

label("$\mathrm{tid} = k$", (0, 4.5), W);
label("$\mathrm{tid} = k + 1$", (0, 3), W);
label("$\mathrm{tid} = k + 2$", (0, 1), W);

path p0 = (0,3.5)..tension 2 ..(3,3.2)..(7,3.8)..(10,3.3);
path p1 = (0,1.3)..tension 1 ..(2,1.6)..(6,1.4)..(10,1.9);

for (real x = 0.3; x <= length(p0); x+= 0.5) {
    draw(projection(a0, a1) * point(p0,x)--point(p0, x), darkcyan, Arrow);
}
for (real x = 0.3; x <= length(p1); x+= 0.5) {
    draw(projection(v3, v2) * point(p1,x)--point(p1, x), darkcyan, Arrow);
}

draw(p0, darkcyan+linewidth(1.1));
draw(p1, darkcyan+linewidth(1.1));
draw(Label("$a_y^{(k + 1)}(\mathrm{TC}_x)$", E), point(p0,length(p0) * 0.9)--(11, 4));
draw(Label("$a_y^{(k + 2)}(\mathrm{TC}_x)$", E), point(p1,length(p1) * 0.9)--(11, 1));

draw(Label("$\vec\mathbf{v}$", 0.5, E), (11, 1.5)--(11,3), linewidth(1.1), Arrow(3mm));

draw(v0--v3--v2--v1);
draw(v0--v1, red + linewidth(1.2));
draw(a0--a1);
draw(b0--b1);
label("$\mathrm{TC}_y = 0$", 0.5 * (v0 + v1), N, red);
