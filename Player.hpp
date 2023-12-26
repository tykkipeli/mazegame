#pragma once
#include <SDL2/SDL.h>
#include "Keyboardinput.hpp"
#include "Map.hpp"

class Player {
public:
    Player();
    Player(double x, double y, double radius);
    
    void load_image(SDL_Renderer *renderer);
    void draw(SDL_Renderer *renderer, float x, float y);
    void update(Keyboardinput *keyboard, Map &map);

    SDL_Texture *image_texture;
    double radius;
    double x,y;
    double speed;
    double angle;
    
private:
    void move_x(double k);
    void move_y(double k);
    
    bool collides_wall(Map &map);
};
