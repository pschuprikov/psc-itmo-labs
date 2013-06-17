import geometry;
import fontsize;
import plain;
size(10cm, 0);
point a = (1,3);
point b = (3,2);
point c = (6,4);
point d = (9,2);
point e = (9,-2);
point f = (15,3);
point g = (9,6);
point h = (3,6);

path coastline = a--b--c--d--e--f--g--h--cycle;

vector cdir = unit(unit(b-c) + unit(d-c));
vector ddir = unit(unit(c-d) + unit(e-d));
line cline = parallel(c,cdir);
line dline = parallel(d,ddir);
point k = intersectionpoint(cline,dline);
vector bdir = -unit(unit(a - b) + unit(c - b));
vector idir = unit(unit(b - c) + unit(e - d));
line bline = parallel(b, bdir);
line kline = parallel(k, idir);
point l = intersectionpoint(bline,kline);
vector ldir = unit(unit(a - b) + unit(e - d));
vector adir = -unit(unit(b - a) + unit(h - a));
vector hdir = -unit(unit(a - h) + unit(g - h));
vector gdir = -unit(unit(h - g) + unit(f - g));
vector fdir = -unit(unit(g - f) + unit(e - f));
vector edir = -unit(unit(d - e) + unit(f - e));

real off = 3;
point ol = l + ldir * off;
point oa = a + adir * off;
point oh = h + hdir * off;
point og = g + gdir * off;
point of = f + fdir * off;
point oe = e + edir * off;


segment offset(point a, point b, point c, point d, real offset) {
    point isec = intersectionpoint(line(a,b), line(c,d));
    real dist = distance(isec, line(a,c));
    transform idtr = scale((offset + dist) / dist, isec);
    return idtr * segment(a, c);
}

vector mov(point a, point b, vector abdir, real offset) {
    real abcos = dot(a - b, abdir) / (length(a - b) * length(abdir));
    real absin = sqrt(1 - abcos*abcos);
    return abdir * offset / absin;
}

string bvec(string name) {
    return "$\vec{\mathbf{" + name + "}}$";
}
