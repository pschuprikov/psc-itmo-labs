include "skeleton_common";

point p = (0,0);
point pp = (5,0);

point dp = unit((-1, 0.7));
point dpp = unit((1., -2.1));

path str = p--pp;
path spl = p{-dp}..{dpp}pp;

point x = point(str,0.7);
point xx = point(spl,0.7);
point xxp = projection(p,pp) * xx;

vector u = (0,1);
show(bvec("u"), u, Arrow(3mm));

dot(bvec("p"), p, W);
dot(bvec("p'"), pp, E);
dot(bvec("x"), x, S);
dot(bvec("x'"), xx, N);

draw(str);
draw(p--(p+dp),dashed+red, Arrow);
draw(pp--(pp+dpp),dashed+red, Arrow);
draw(spl, red);
draw("$\mathrm{\Delta TC}_y$", xxp--xx, blue, Arrow);


