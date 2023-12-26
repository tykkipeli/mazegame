#include "Player.hpp"
#include "physics.hpp"
#include "utils.hpp"
#include <iostream>


Player::Player()
{}


Player::Player(double x, double y, double radius) {
    this->x = x;
    this->y = y;
    this->radius = radius;
    this->speed = 3;
}


// debuggausta varten
void draw_circle(SDL_Renderer *renderer, double radius, double x, double y) {
    SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
    for (double x2 = 0; x2 < 1000; x2++) {
        for (double y2 = 0; y2 < 1000; y2++) {
            double dist = (x-x2)*(x-x2) + (y-y2)*(y-y2);
            if (dist < radius*radius) SDL_RenderDrawPointF(renderer, x2, y2);
        }
    }
}

void Player::draw(SDL_Renderer *renderer, float x, float y) {
    SDL_FRect dest{(float) (x - radius), (float) (y - radius), (float) (2*radius), (float) (2*radius)};
    //draw_circle(renderer, radius, x, y);
    SDL_RenderCopyExF(renderer, this->image_texture, nullptr, &dest, angle*(180.0/PII) - 96, nullptr, SDL_FLIP_NONE);
}

void Player::load_image(SDL_Renderer *renderer) {
    this->image_texture = load_texture("images/nuuhkis-modified.bmp", renderer);
}

bool Player::collides_wall(Map &map) {
    auto walls = map.get_close_walls(this->x, this->y, 1);
    for (auto rect : walls) {
        if (circle_rect_collision(this->x, this->y, this->radius, rect.x, rect.y, rect.w, rect.h)) return true;
    }
    return false;
}

void Player::update(Keyboardinput *keyboard, Map &map) {
    if (keyboard->shift) speed = 6;
    else speed = 3;
    if (keyboard->left) angle -= 0.08;
    if (keyboard->right) angle += 0.08;
    if (keyboard->up || keyboard->down) {
        double k = -0.33;
        if (keyboard->up) k = 1;
        for (int i = 0; i < speed; i++) {
            this->move_x(k);
            if (this->collides_wall(map)) this->move_x(-k);
            this->move_y(k);
            if (this->collides_wall(map)) this->move_y(-k);
        }
    }
}

void Player::move_x(double k) {
    double dx = std::cos(angle);
    this->x += k*dx;
}

void Player::move_y(double k) {
    double dy = std::sin(angle);
    this->y += k*dy;
}
