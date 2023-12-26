#pragma once
#include <vector>
#include <SDL2/SDL.h>


extern const double PII;

template <typename T> void shuffle(std::vector<T> &v);
//void shuffle(std::vector<std::pair<int,int>> &v);

std::pair<int,int> choice(std::vector<std::pair<int,int>> &v);
bool contains(const std::vector<std::pair<int,int>>&v, std::pair<int,int> p);
SDL_Texture * load_texture(const char* file, SDL_Renderer *renderer);
double towards_angle(double angle, double target_angle, double amount);
