#include "Keyboardinput.hpp"


Keyboardinput::Keyboardinput() {}


void Keyboardinput::handle_keydown(SDL_Keycode key) {
    if (key == SDLK_RIGHT) right = true;
    else if (key == SDLK_LEFT) left = true;
    else if (key == SDLK_UP) up = true;
    else if (key == SDLK_DOWN) down = true;
    else if (key == SDLK_LSHIFT) shift = true;
}

void Keyboardinput::handle_keyup(SDL_Keycode key) {
    if (key == SDLK_RIGHT) right = false;
    else if (key == SDLK_LEFT) left = false;
    else if (key == SDLK_UP) up = false;
    else if (key == SDLK_DOWN) down = false;
    else if (key == SDLK_LSHIFT) shift = false;
}

void Keyboardinput::handle_mousemotion(int x, int y) {
    mouse_x = x;
    mouse_y = y;
}

void Keyboardinput::handle_mouseclick(int x, int y) {
    clicked = true;
}

Keyboardinput * create_keyboardinput() {
    Keyboardinput * res = new Keyboardinput();
    return res;
}
