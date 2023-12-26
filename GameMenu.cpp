#include "GameMenu.hpp"
#include <iostream>

GameMenu::GameMenu() {
    this->text_texture = nullptr;
    this->current_texture = nullptr;
    this->hover_texture = nullptr;
    //this->start_button = {0,0,0,0};
}

void GameMenu::init(SDL_Renderer *renderer, int screenwidth, int screenheight) {
    this->screenwidth = screenwidth;
    this->screenheight = screenheight;
    this->load_textures(renderer);
    int width = screenwidth/5;
    int gap = width + screenwidth/10;
    int offset = screenwidth/5;
    start_button.init(renderer, offset, screenheight/2, width);
    load_button.init(renderer, offset + gap, screenheight/2, width);
    settings_button.init(renderer, offset + 2*gap, screenheight/2, width);
}

void GameMenu::load_textures(SDL_Renderer *renderer) {
    start_button.load_textures(renderer, "Start");
    load_button.load_textures(renderer, "Load");
    settings_button.load_textures(renderer, "Settings");
}

void GameMenu::draw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255,0,255, 255);
    SDL_RenderClear(renderer);
    start_button.draw(renderer);
    settings_button.draw(renderer);
    load_button.draw(renderer);
}

bool GameMenu::update(Keyboardinput *keyboard) {
    if (start_button.update(keyboard)) return true;
    load_button.update(keyboard);
    settings_button.update(keyboard);
    return false;
}
