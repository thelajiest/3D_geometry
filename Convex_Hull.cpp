/*
 * @Author: yuri3
 * @Date: 2021-04-09 17:41:27
 */

#include <bits/stdc++.h>
using namespace std;
const int MAXN = 505;
const double EPS = 1e-8;
struct Point {
    double x, y, z;
    Point() {}
    Point(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {}

    Point operator-(const Point p1) {
        return Point(x - p1.x, y - p1.y, z - p1.z);
    }

    Point operator*(Point p) {
        return Point(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
    }

    double operator^(Point p) { return (x * p.x + y * p.y + z * p.z); }
};

struct CH3D {
    /*
     * API：
     *      input:  n:
     *              p[i].x p[i].y p[i].z:
     *              solve(): build
     *      return: double area(): Area of Convex Hull
     *              double volume(): Volume of Convex Hull
     *              int polygon():
     *              int polygon():
     *              Point getcent(): ccnter of Convex Hull
     *              double function(Point Temp): distance between Point Temp and
     * surface
     *
     */

    struct face {
        int a, b, c;
        bool ok;
    };

    int n;
    Point P[MAXN];

    int num;
    face F[8 * MAXN];

    int g[MAXN][MAXN];

    double vlen(Point a) { return sqrt(a.x * a.x + a.y * a.y + a.z * a.z); }

    Point cross(const Point &a, const Point &b, const Point &c) {
        return Point((b.y - a.y) * (c.z - a.z) - (b.z - a.z) * (c.y - a.y),
                     -((b.x - a.x) * (c.z - a.z) - (b.z - a.z) * (c.x - a.x)),
                     (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
    }
    double area(Point a, Point b, Point c) { return vlen((b - a) * (c - a)); }

    double volume(Point a, Point b, Point c, Point d) {
        return (b - a) * (c - a) ^ (d - a);
    }

    double dblcmp(Point &p, face &f) {
        Point m = P[f.b] - P[f.a];
        Point n = P[f.c] - P[f.a];
        Point t = p - P[f.a];
        return (m * n) ^ t;
    }

    void deal(int p, int a, int b) {
        int f = g[a][b];
        face add;
        if (F[f].ok) {
            if (dblcmp(P[p], F[f]) > EPS)
                dfs(p, f);
            else {
                add.a = b;
                add.b = a;
                add.c = p;
                add.ok = 1;
                g[p][b] = g[a][p] = g[b][a] = num;
                F[num++] = add;
            }
        }
    }

    void dfs(int p, int now) {
        F[now].ok = 0;
        deal(p, F[now].b, F[now].a);
        deal(p, F[now].c, F[now].b);
        deal(p, F[now].a, F[now].c);
    }

    bool same(int s, int t) {
        Point &a = P[F[s].a];
        Point &b = P[F[s].b];
        Point &c = P[F[s].c];
        return fabs(volume(a, b, c, P[F[t].a])) < EPS &&
               fabs(volume(a, b, c, P[F[t].b])) < EPS &&
               fabs(volume(a, b, c, P[F[t].c])) < EPS;
    }

    void solve() {
        int i, j, tmp;
        face add;
        bool flag = true;
        num = 0;
        if (n < 4) return;
        for (i = 1; i < n; i++) {
            if (vlen(P[0] - P[i]) > EPS) {
                swap(P[1], P[i]);
                flag = false;
                break;
            }
        }
        if (flag) return;
        flag = true;
        for (i = 2; i < n; i++) {
            if (vlen((P[0] - P[1]) * (P[1] - P[i])) > EPS) {
                swap(P[2], P[i]);
                flag = false;
                break;
            }
        }
        if (flag) return;
        flag = true;
        for (i = 3; i < n; i++) {
            if (fabs((P[0] - P[1]) * (P[1] - P[2]) ^ (P[0] - P[i])) > EPS) {
                swap(P[3], P[i]);
                flag = false;
                break;
            }
        }
        if (flag) return;
        for (i = 0; i < 4; i++) {
            add.a = (i + 1) % 4;
            add.b = (i + 2) % 4;
            add.c = (i + 3) % 4;
            add.ok = true;
            if (dblcmp(P[i], add) > 0) swap(add.b, add.c);
            g[add.a][add.b] = g[add.b][add.c] = g[add.c][add.a] = num;
            F[num++] = add;
        }
        for (i = 4; i < n; i++) {
            for (j = 0; j < num; j++) {
                if (F[j].ok && dblcmp(P[i], F[j]) > EPS) {
                    dfs(i, j);
                    break;
                }
            }
        }
        tmp = num;
        for (i = num = 0; i < tmp; i++)
            if (F[i].ok) {
                F[num++] = F[i];
            }
    }

    double area() {
        double res = 0.0;
        if (n == 3) {
            Point p = cross(P[0], P[1], P[2]);
            res = vlen(p) / 2.0;
            return res;
        }
        for (int i = 0; i < num; i++)
            res += area(P[F[i].a], P[F[i].b], P[F[i].c]);
        return res / 2.0;
    }

    double volume() {
        double res = 0.0;
        Point tmp(0, 0, 0);
        for (int i = 0; i < num; i++)
            res += volume(tmp, P[F[i].a], P[F[i].b], P[F[i].c]);
        return fabs(res / 6.0);
    }

    int triangle() { return num; }

    int polygon() {
        int i, j, res, flag;
        for (i = res = 0; i < num; i++) {
            flag = 1;
            for (j = 0; j < i; j++)
                if (same(i, j)) {
                    flag = 0;
                    break;
                }
            res += flag;
        }
        return res;
    }
    Point getcent() {
        Point ans(0, 0, 0), temp = P[F[0].a];
        double v = 0.0, t2;
        for (int i = 0; i < num; i++) {
            if (F[i].ok == true) {
                Point p1 = P[F[i].a], p2 = P[F[i].b], p3 = P[F[i].c];
                t2 = volume(temp, p1, p2, p3) / 6.0;
                if (t2 > 0) {
                    ans.x += (p1.x + p2.x + p3.x + temp.x) * t2;
                    ans.y += (p1.y + p2.y + p3.y + temp.y) * t2;
                    ans.z += (p1.z + p2.z + p3.z + temp.z) * t2;
                    v += t2;
                }
            }
        }
        ans.x /= (4 * v);
        ans.y /= (4 * v);
        ans.z /= (4 * v);
        return ans;
    }
    double function(Point Temp) {
        double min = 99999999;
        for (int i = 0; i < num; i++) {
            if (F[i].ok == true) {
                Point p1 = P[F[i].a], p2 = P[F[i].b], p3 = P[F[i].c];
                double a = ((p2.y - p1.y) * (p3.z - p1.z) -
                            (p2.z - p1.z) * (p3.y - p1.y));
                double b = ((p2.z - p1.z) * (p3.x - p1.x) -
                            (p2.x - p1.x) * (p3.z - p1.z));
                double c = ((p2.x - p1.x) * (p3.y - p1.y) -
                            (p2.y - p1.y) * (p3.x - p1.x));
                double d = (0 - (a * p1.x + b * p1.y + c * p1.z));
                double temp = fabs(a * Temp.x + b * Temp.y + c * Temp.z + d) /
                              sqrt(a * a + b * b + c * c);
                if (temp < min) min = temp;
            }
        }
        return min;
    }
};
