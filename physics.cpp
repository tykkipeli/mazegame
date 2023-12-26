#include "physics.hpp"
#include <vector>
#include <cmath>

using PDD = std::pair<double,double>;

bool circle_rect_collision(double circ_x, double circ_y, double radius, double x, double y, double w, double h) {
    std::vector<PDD> corners = {{x,y}, {x+w,y}, {x,y+h}, {x+w,y+h}};
    for (auto p : corners) {
        double dist_squared = (circ_x-p.first)*(circ_x-p.first) + (circ_y-p.second)*(circ_y-p.second);
        if (dist_squared < radius*radius) return true;
    }
    // Jos kesikipsite ala ja yläreunan välissä
    if (circ_y >= y && circ_y <= y+h) {
        if (circ_x - (x+w) >= radius) return false;
        if (x - circ_x >= radius) return false;
        return true;
    }
    // jos keskipiste vasemman ja oikean reunan välissä
    if (circ_x >= x && circ_x <= x+w) {
        if (circ_y - (y+h) >= radius) return false;
        if (y - circ_y >= radius) return false;
        return true;
    }
    return false;
}

inline PDD diff(const PDD &p1, const PDD &p2) {
    return {p1.first - p2.first, p1.second - p2.second};
}

inline double dotp(const PDD &p1, const PDD &p2) {
    return p1.first*p2.first + p1.second*p2.second;
}

inline double cross(const PDD &p1, const PDD &p2) {
    return p1.first*p2.second - p1.second*p2.first;
}


PDD circle_segment_intersection(const PDD &a, const PDD &b, const PDD &c, double r) {
    PDD d = diff(b, a);
    PDD f = diff(a, c);
    double A = dotp(d,d);
    double B = 2*dotp(f,d);
    double C = dotp(f,f) - r*r;
    double discriminant = B*B - 4*A*C;
    if (discriminant < 0) return {-1e9, -1e9};
    discriminant = sqrt(discriminant);
    double t1 = (-B - discriminant)/(2*A);
    double t2 = (-B + discriminant)/(2*A);
    if (t1 >= 0 && t1 <= 1) return {a.first + t1*d.first, a.second + t1*d.second};
    if (t2 >= 0 && t2 <= 1) return {a.first + t2*d.first, a.second + t2*d.second};
    return {-1e9, -1e9};
}

PDD segment_intersection(const PDD &a, const PDD &b, const PDD &c, const PDD &d) {
    auto r = diff(b,a);
    auto s = diff(d,c);
    double dd = cross(r,s);
    if (dd == 0) return {-1e9, -1e9};
    double t = cross(diff(c,a), s)/dd;
    double u = cross(diff(c,a), r)/dd;
    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) return {a.first + t*r.first, a.second + t*r.second};
    return {-1e9, -1e9};
}

PDD closest_intersection(const PDD &a, const PDD &b, const std::vector<std::pair<PDD, PDD>> &segments) {
    std::vector<PDD> collision_points;
    PDD NONE = {-1e9, -1e9};
    for (auto p : segments) {
        auto intersection = segment_intersection(a,b, p.first, p.second);
        if (intersection != NONE) collision_points.push_back(intersection);
    }
    //std::pair<double,double> collision_point = NONE;
    PDD collision_point = NONE;
    double best_dist = 1e18;
    if (collision_points.size() > 0) {
        for (auto p : collision_points) {
            double dist = distance(p, a);
            //double dist = (p.first-a.first)*(p.first-a.first) + (p.second-a.second)*(p.second-a.second);
            if (dist < best_dist) {
                best_dist = dist;
                collision_point = p;
            }
        }
    }
    return collision_point;
}

/*
#segmenttien [a,b] ja [c,d] leikkauspiste
#leikkauspisteelle pätee a + t*r = c + u*s
def segment_intersection(a, b, c, d):
    #[a,b]:n suuntavektori:
    r = diff(b,a)
    #[c,d]:n suuntavektori:
    s = diff(d,c)
    #nimittäjä:
    d = cross(r,s)
    if d == 0:
        return None
    t = cross(diff(c,a), s) / d
    u = cross(diff(c,a), r) / d
    if t >= 0 and t <= 1 and u >= 0 and u <= 1:
        return (a[0] + t*r[0], a[1] + t*r[1])
    return None


# p1 - p2
def diff(p1, p2):
    return (p1[0]-p2[0], p1[1]-p2[1])

def cross(p1, p2):
    return p1[0]*p2[1] - p1[1]*p2[0]
*/
