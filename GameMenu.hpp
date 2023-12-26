#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Keyboardinput.hpp"
#include "Button.hpp"

class GameMenu {
public:    
    GameMenu();
    
    void draw(SDL_Renderer *renderer);
    bool update(Keyboardinput *keyboard);
    void init(SDL_Renderer *renderer, int screenwidth, int screenheight);
    
    
private:
    SDL_Texture *current_texture;
    SDL_Texture *text_texture;
    SDL_Texture *hover_texture;
    Button start_button;
    Button load_button;
    Button settings_button;
    int screenwidth = 0;
    int screenheight = 0;
    
    void load_textures(SDL_Renderer *renderer);
};
