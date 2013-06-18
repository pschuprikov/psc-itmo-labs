include "skeleton_common.asy";

real dist = distance(k, line(b,c));
point m = b + mov(b, c, bdir, dist);
point xy = (m * 0.2 + k * 0.8) * 0.4 + (b * 0.4 + c * 0.6) * 0.6;
real distxy = distance(xy, line(b,c));
point p = b + mov(b, c, bdir, distxy);
point pp = c + mov(b, c, cdir, distxy);

point perp = unit(c - b);
perp = (-perp.y, perp.x);
coordsys csbc = cartesiansystem(b, i=unit(c - b), j=perp);

string desc(string l) {
    return bvec(l) +  "$\{\mathrm{TC}_x^{" + l 
                   + "},\mathrm{TC}_y^{" + l 
                   + "}\}$";
}

fill(b--c--k--m--cycle, lightgreen);
fill(k--m--l--cycle, lightblue);

dot(desc("b"),b,W);
dot(desc("c"),c,SE);
dot(desc("k"),k,E);
dot(desc("l"),l,E);
dot(desc("m"),m,W);
dot(desc("p"),p,W);
dot(desc("p'"),pp,E);
draw(c--k,dashed);
draw(b--l,dashed);
draw(c--k,dashed);
draw(l--k,dashed);
draw(b--c,linewidth(2));
draw(m--k);
draw(p--pp);

show(bvec("u"), vector(csbc, (0.,1.)), red+linewidth(2.), Arrow(3mm));
show(bvec("s"), vector(csbc, (1.,0.)), red+linewidth(2.), Arrow(3mm));

dot(bvec("x"), xy, NW);
