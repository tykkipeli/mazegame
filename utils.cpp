#include "utils.hpp"
#include <stdlib.h>
#include <iostream>

const double PII = 3.141592653589793238463;


template <typename T> void shuffle(std::vector<T> &v) {
    int n = v.size();
    for (int i = 0; i < n; i++) {
        int x = rand()%(n-i);
        std::swap(v[i], v[i+x]);
    }
}
template void shuffle<std::pair<int,int>>(std::vector<std::pair<int,int>> &v);
template void shuffle<int>(std::vector<int> &v);

/*
void shuffle(std::vector<std::pair<int,int>> &v) {
    int n = v.size();
    for (int i = 0; i < n; i++) {
        int x = rand()%(n-i);
        swap(v[i], v[i+x]);
    }
}
*/

std::pair<int,int> choice(std::vector<std::pair<int,int>> &v) {
    int n = v.size();
    int ind = rand()%n;
    return v[ind];
}

bool contains(const std::vector<std::pair<int,int>>&v, std::pair<int,int> p) {
    for (auto pp : v) {
        if (pp == p) return true;
    }
    return false;
}

SDL_Texture * load_texture(const char* file, SDL_Renderer *renderer) {
    SDL_Surface* Loading_Surf = SDL_LoadBMP(file);
    if (Loading_Surf == NULL) {
        std::cout << "KUVAN LATAAMINEN EI ONNISTUNUT!" << std::endl;
    }
    auto texture = SDL_CreateTextureFromSurface(renderer, Loading_Surf);
    SDL_FreeSurface(Loading_Surf);
    return texture;
}

double towards_angle(double angle, double target_angle, double amount) {
    double pos_dist, neg_dist;
    if (angle > target_angle) {
        pos_dist = angle - target_angle;
        neg_dist = target_angle+2*PII - angle;
    } else {
        neg_dist = target_angle - angle;
        pos_dist = angle + 2*PII - target_angle;
    }
    //std::cout << pos_dist << " " << neg_dist << std::endl;
    if (pos_dist < neg_dist) angle -= amount;
    else angle += amount;
    if (angle > PII) angle -= 2*PII;
    if (angle < -PII) angle += 2*PII;
    return angle;
}
