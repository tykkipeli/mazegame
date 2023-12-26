#include "Button.hpp"
#include <iostream>

Button::Button() {
    this->text_texture = nullptr;
    this->current_texture = nullptr;
    this->hover_texture = nullptr;
    this->area = {0,0,0,0};
}

void Button::init(SDL_Renderer *renderer, int x, int y, int width) {
    //this->load_textures(renderer);
    int w, h;
    SDL_QueryTexture(current_texture, NULL, NULL, &w, &h);
    std::cout << "w: " << w << " width: " << width << std::endl;
    std::cout << w <<  " " << h << std::endl;
    double height = (double)h* (double)width/(double)w;
    std::cout << "height: " << height << std::endl;
    h = height;
    w = width;
    this->area = {x-w/2, y-h/2, w, h};
    std::cout << x-w/2 << " " << y-h/2 << " " <<  w << " " <<  h << std::endl;
}

void Button::draw(SDL_Renderer *renderer) {
    SDL_RenderCopy(renderer, current_texture, NULL, &area);
}

bool Button::update(Keyboardinput *keyboard) {
    SDL_Point point = {keyboard->mouse_x, keyboard->mouse_y};
    if (SDL_PointInRect(&point, &area)) {
        this->current_texture = this->hover_texture;
        if (keyboard->clicked) return true;
    } else {
        this->current_texture = this->text_texture;
    }
    return false;
}


void Button::load_textures(SDL_Renderer *renderer, const char *text) {
    TTF_Font  *font = TTF_OpenFont("acme.ttf", 200);
    SDL_Surface *textSurf = TTF_RenderText_Solid(font, text, {0,0,0});
    this->text_texture = SDL_CreateTextureFromSurface(renderer, textSurf);
    SDL_Surface *hoverSurf = TTF_RenderText_Solid(font, text, {255,0,0});
    this->hover_texture = SDL_CreateTextureFromSurface(renderer, hoverSurf);
    SDL_FreeSurface(hoverSurf);
    SDL_FreeSurface(textSurf);
    TTF_CloseFont(font);
    this->current_texture = this->text_texture;
}
