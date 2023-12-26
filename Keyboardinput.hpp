#pragma once
#include <SDL2/SDL.h>

class Keyboardinput {
public:
    Keyboardinput();

    void handle_keydown(SDL_Keycode key);
    void handle_keyup(SDL_Keycode key);
    void handle_mousemotion(int x, int y);
    void handle_mouseclick(int x, int y);
    
    bool right = false;
    bool left = false;
    bool up = false;
    bool down = false;
    bool shift = false;
    
    int mouse_x = 0;
    int mouse_y = 0;
    bool clicked = false;
    
private:
    
};

Keyboardinput * create_keyboardinput();
