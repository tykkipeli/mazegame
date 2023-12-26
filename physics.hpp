#pragma once
#include <vector>

using PDD = std::pair<double,double>;

bool circle_rect_collision(double circ_x, double circ_y, double radius, double rect_x, double rect_y, double rect_width, double rect_height);

// segmenttien [a,b] ja [c,d] leikkauspiste (tai {-1e9, -1e9} jos eivät leikkaa)
PDD segment_intersection(const PDD &a, const PDD &b, const PDD &c, const PDD &d);

// segmentti [a,b] ja joukko muita segmenttejä segments
// palauttaa leikkauspisteen lähimpänä pistettä a
PDD closest_intersection(const PDD &a, const PDD &b, const std::vector<std::pair<PDD, PDD>> &segments);


// segmentti [a,b] ja ympyrä, jonka keskipiste on c ja säde r
// palauttaa leikkauspisteen, joka on lähinnä pistettä a (tai {-1e9, -1e9} jos eivät leikkaa)
PDD circle_segment_intersection(const PDD &a, const PDD &b, const PDD &c, double r);


// squared_distance
inline double distance(const PDD &p1, const PDD &p2) {
    return (p1.first-p2.first)*(p1.first-p2.first) + (p1.second-p2.second)*(p1.second-p2.second);
}
