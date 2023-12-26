#pragma once
#include <SDL2/SDL.h>
#include "Map.hpp"
#include <vector>

using CIRCLE_LIST = std::vector<std::pair<std::pair<double,double>,double>>;

class Monster {
public:
    Monster();
    Monster(double x, double y, double radius);
    
    void load_image(SDL_Renderer *renderer);
    void draw(SDL_Renderer *renderer, double player_x, double player_y, int screenwidth, int screenheight);
    void draw_rays(SDL_Renderer *renderer, double player_x, double player_y, int screenwidth, int screenheight);
    void update(Map &map);
    void update_rays(Map &map, CIRCLE_LIST &circles);

    SDL_Texture *image_texture;
    double radius;
    double x,y;
    double speed;
    double angle;
    double angle_speed;
    
private:
    void move_x(double k);
    void move_y(double k);
    
    bool collides_wall(Map &map);
    void move(Map &map);
    void change_target(Map &map);
    
    double target_x;
    double target_y;
    // previous target positions:
    double prev_x;
    double prev_y;
    double target_angle;
    
    std::vector<std::pair<std::pair<double,double>, std::pair<double,double>>> rays;
};
