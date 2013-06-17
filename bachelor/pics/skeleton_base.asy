import geometry;
import fontsize;
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

real soff = 0.5;
point dd = d + mov(d, e, ddir, soff);
point ee = e + mov(e, f, edir, soff);
point ff = f + mov(f, g, fdir, soff);
point gg = g + mov(g, h, gdir, soff);
point hh = h + mov(h, a, hdir, soff);
point aa = a + mov(a, b, adir, soff);
point bb = b + mov(b, c, bdir, soff);
point cc = c + mov(c, d, cdir, soff);
real soff0 = 1;
point dd0 = d + mov(d, e, ddir, soff0);
point ee0 = e + mov(e, f, edir, soff0);
point ff0 = f + mov(f, g, fdir, soff0);
point gg0 = g + mov(g, h, gdir, soff0);
point hh0 = h + mov(h, a, hdir, soff0);
point aa0 = a + mov(a, b, adir, soff0);
point bb0 = b + mov(b, c, bdir, soff0);
point cc0 = c + mov(c, d, cdir, soff0);

path coast0 = aa--bb--cc--dd--ee--ff--gg--hh--cycle;
path coast1 = aa0--bb0--cc0--dd0--ee0--ff0--gg0--hh0--cycle;

draw(coast0, green, EndArrow(5, position=0.8));
draw(coast1, cyan, EndArrow(5, position=0.8));
draw(a--((a + oa) * 0.5), blue + linewidth(1.2));
draw(coastline, solid+linewidth(1.2)+red);
label("$\mathrm{TC}_x=0$", a+((oa-a) * 0.5), NW, blue);
label("$\mathrm{TC}_y=0$", (c + d) * 0.5, NE, red);
draw(Label("$l(y_0)$", 0), (point(coast0, 4.5) + (1,0))--point(coast0, 4.5), green); 
draw(Label("$l(y_1)$", 0), (point(coast1, 4.2) + (1,0))--point(coast1, 4.2), cyan); 

dot("$a$",a,S);
dot("$b$",b,N);
dot("$c$",c,N);
dot("$d$",d,NE);
dot("$e$",e,W);
dot("$f$",f,W);
dot("$g$",g,S);
dot("$h$",h,S);
dot("$k$",k,W);
dot("$l$",l,E);
draw(c--k,dashed);
draw(d--k,dashed);
draw(b--l,dashed);
draw(k--l,dashed);
draw(l--ol,dashed);
draw(a--oa,dashed);
draw(h--oh,dashed);
draw(g--og,dashed);
draw(f--of,dashed);
draw(e--oe,dashed);
