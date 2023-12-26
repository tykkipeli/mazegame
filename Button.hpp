#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Keyboardinput.hpp"

class Button {
public:    
    Button();
    
    void init(SDL_Renderer *renderer, int x, int y, int width); 
    void draw(SDL_Renderer *renderer);
    bool update(Keyboardinput *keyboard);
    void load_textures(SDL_Renderer *renderer, const char *text);
    
    
private:
    SDL_Texture *current_texture;
    SDL_Texture *text_texture;
    SDL_Texture *hover_texture;
    SDL_Rect area;
};
