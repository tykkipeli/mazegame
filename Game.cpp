#include "Game.hpp"
#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>

using CIRCLE = std::pair<std::pair<double,double>,double>;


Game::Game()
{}

            
void Game::generate_monsters(int level) {   
    for (auto monster : this->monsters) delete monster;
    this->monsters.clear();
    double gap = 3*SIZE;
    int monta = (map.h*map.w)/20;
    //monta = 5;
    for (int itr = 0; itr < monta; itr++) {
        int i = rand()%(map.w);
        int j = rand()%(map.h);
        double radius = 0.6*SIZE;
        //TODO kutsu mieluummin map.get_random_position
        double padding = map.wall_thickness/2 + radius + 1;
        double ilisa = padding + rand()%((int)(gap-2*padding));
        double jlisa = padding + rand()%((int)(gap-2*padding));
        double mons_x = gap*i + ilisa;
        double mons_y = gap*j + jlisa;
        Monster *mons = new Monster(mons_x, mons_y, radius);
        mons->load_image(renderer);
        monsters.push_back(mons);
    }
}

void Game::init_level(int level) {
    this->map = Map(SIZE);
    int koko = 5;
    this->map.generate(koko, koko);
    double gap = 3*SIZE;
    int i = rand()%(map.w);
    int j = rand()%(map.h);
    //this->player = Player(1.5*SIZE + gap*i, 1.5*SIZE + gap*j, 0.4*SIZE);
    this->player = Player(1.5*SIZE + gap*i, 1.5*SIZE + gap*j, 0.4*SIZE);
    this->player.load_image(renderer);
    this->generate_monsters(level);
}

void Game::create_shadow_texture() {
    shadow_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, screenwidth, screenheight);
    SDL_SetRenderTarget(renderer, shadow_texture);
    SDL_SetTextureBlendMode(shadow_texture, SDL_BLENDMODE_BLEND); 
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer); 
    int x = screenwidth/2;
    int y = screenheight/2;
    int radius = 200;
    for (int i = 0; i < screenwidth; i++) {
        for (int j = 0; j < screenheight; j++) {
            int dist = (i-x)*(i-x) + (j-y)*(j-y);
            if (dist > radius*radius) {
                
                int blend = std::min(255, (dist-radius*radius)/300);
                /*
                int ero = dist-radius*radius;
                int blend = std::min(255, (int)(sqrt(sqrt(ero))*13));
                */
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, blend);
                if (blend != 255) std::cout << blend << std::endl;
                //SDL_SetRenderDrawColor(renderer, 0, 255, 0, 200);
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }
    }
}

void Game::init(const char* title, int width, int height, int SIZE) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    this->renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    this->keyboard = create_keyboardinput();
    this->isRunning = true;
    this->screenwidth = width;
    this->screenheight = height;
    this->SIZE = SIZE;
    this->create_shadow_texture();
    this->state = "menu";
    this->menu.init(renderer, width, height);
    std::cout << " menu alustettu" << std::endl;
    srand(time(NULL));
    //this->init_level(1);
}

void Game::handleEvents() {
    keyboard->clicked = false;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            //std::cout << "Closing" << std::endl;
            this->isRunning = false;
            break;
        case SDL_KEYDOWN:
            keyboard->handle_keydown(event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            keyboard->handle_keyup(event.key.keysym.sym);
            break;
        case SDL_MOUSEMOTION:
            keyboard->handle_mousemotion(event.motion.x, event.motion.y);
            break;
        case SDL_MOUSEBUTTONUP:
            std::cout << "Klikkasit kohdasta " << event.button.x << " " << event.button.y << std::endl;
            keyboard->handle_mouseclick(event.button.x, event.button.y);
            break;
        default:
            break;
        }
    }
}

bool Game::running() {
    return this->isRunning;
}

bool monster_is_close(Monster *monster, Player &player, double SIZE) {
    return std::abs(monster->x - player.x) <= 30*SIZE && std::abs(monster->y - player.y) <= 30*SIZE;
}

void Game::update() {
    if (this->state == "menu") {
        if (this->menu.update(keyboard)) {
            this->init_level(1);
            this->state = "playing";
        }
    } else {
        player.update(keyboard, this->map);
        map.update(player.x, player.y, screenwidth, screenheight);
        std::vector<CIRCLE> close_circles;
        for (auto monster : this->monsters) {
            monster->update(this->map);
            if (monster_is_close(monster, player, SIZE)) {
                close_circles.push_back({{monster->x, monster->y}, monster->radius});
            }
        }
        close_circles.push_back({{player.x, player.y}, player.radius});
        for (auto monster : this->monsters) {
            if (monster_is_close(monster, player, SIZE)) {
                monster->update_rays(this->map, close_circles);
            }
        }
    }
}

void draw_shadow(SDL_Renderer *renderer, int width, int height) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    int x = width/2;
    int y = height/2;
    int radius = 10;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int dist = (i-x)*(i-x) + (j-y)*(j-y);
            if (dist < radius*radius) SDL_RenderDrawPoint(renderer, i, j);
        }
    }
}

void Game::render() {
    SDL_SetRenderTarget(renderer, NULL);
    if (this->state == "menu") {
        this->menu.draw(renderer);
    } else {
        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
        SDL_RenderClear(renderer);
        map.draw(renderer, player.x, player.y, screenwidth, screenheight);
        int monta = 0;
        for (auto monster : this->monsters) {
            if (std::abs(monster->x - player.x) <= 30*SIZE && std::abs(monster->y - player.y) <= 30*SIZE) {
                monster->draw_rays(renderer, player.x, player.y, screenwidth, screenheight);
                monta++;
            }
            if (std::abs(monster->x - player.x) <= screenwidth/2 + monster->radius && std::abs(monster->y - player.y) <= screenheight/2 + monster->radius) {
                monster->draw(renderer, player.x, player.y, screenwidth, screenheight);
            }
        }
        player.draw(renderer, screenwidth/2, screenheight/2);
        //draw_shadow(renderer, screenwidth, screenheight);
        SDL_RenderCopy(renderer, shadow_texture, NULL, NULL);
    }
    
    /*
    SDL_FRect r = {100, 100, 300, 300};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 10);
    SDL_RenderFillRectF(renderer, &r);
    */
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    for (auto monster : this->monsters) delete monster;
    delete keyboard;
    std::cout << "clean called" << std::endl;
    SDL_DestroyRenderer(renderer);
    std::cout << "renderer destroyed" << std::endl;
    SDL_DestroyWindow(window);
    std::cout << "window destroyed" << std::endl;
    TTF_Quit();
    SDL_Quit();
    std::cout << "SDL quited" << std::endl;
}
