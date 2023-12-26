#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Map.hpp"
#include "Player.hpp"
#include "Monster.hpp"
#include "Keyboardinput.hpp"
#include "GameMenu.hpp"
#include <vector>
#include <string>

class Game {
public:
    SDL_Renderer *renderer;
    SDL_Event event;
    Keyboardinput *keyboard;
    bool isRunning = false;
    int screenwidth = 0, screenheight = 0;
    int SIZE = 0;
    
    Map map;
    Player player;
    GameMenu menu; 
    std::vector<Monster *> monsters;
    
    Game();
    void init(const char* title, int width, int height, int SIZE);

    void handleEvents();
    void update();
    bool running();
    void render();
    void clean();

    //std::string state = "playing";

private:
    SDL_Window *window;
    SDL_Texture *shadow_texture;
    std::string state = "menu";
    
    void init_level(int level);
    void create_shadow_texture();
    void generate_monsters(int level);
};
