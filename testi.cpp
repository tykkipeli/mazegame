#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
// compile with g++ testi.cpp -lSDL2 -lSDL2main -o testi

using namespace std;

class Game {
public:
    Game();
    ~Game();
    
private:
    
};

#define WIDTH 800
#define HEIGHT 800

void draw_background(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderClear(renderer);
}

void draw_rect(SDL_Renderer *renderer, int size, int x, int y) {
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = size;
    r.h = size;
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderFillRect(renderer, &r);
}

void draw_circle(SDL_Renderer *renderer, int radius) {
    SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
    int x = WIDTH/2;
    int y = HEIGHT/2;
    for (int x2 = 0; x2 < WIDTH; x2++) {
        for (int y2 = 0; y2 < HEIGHT; y2++) {
            int dist = (x-x2)*(x-x2) + (y-y2)*(y-y2);
            if (dist < radius*radius) SDL_RenderDrawPoint(renderer, x2, y2);
        }
    }
}

int main()
{
    bool quit = false;
    SDL_Event e;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Nuuh nuuuuh!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                         "Tärkeä ilmoitus!",
                         "Rapsuhätä kriittinen! Anna välittömästi Nuuhkikselle rapsuja!!",
                         NULL);

    draw_background(renderer);
    draw_rect(renderer, 50, 50, 50);
    
    draw_circle(renderer, 50);
    
    /*
    SDL_Texture *texture = NULL;
    IMG_Init(IMG_INIT_PNG);
    texture = IMG_LoadTexture(renderer, "images/nuuhkis_modified.png");
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    */
    
    SDL_RenderPresent(renderer);
    
    
    const char *image_path = "images/nuuhkis-modified.bmp";
    SDL_Surface *image = SDL_LoadBMP(image_path);
    SDL_UpdateWindowSurface( window );

    /*
    SDL_Surface* winSurface = SDL_GetWindowSurface( window );

    // do drawing
    SDL_FillRect( winSurface, NULL, SDL_MapRGB( winSurface->format, 255, 90, 120 ));

    SDL_UpdateWindowSurface( window );
    */
    
    auto red_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
    auto blue_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
    SDL_SetRenderTarget(renderer, red_texture);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderTarget(renderer, blue_texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderTarget(renderer, nullptr);
    
    SDL_RenderCopy(renderer, red_texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    SDL_Delay(500);
    
    SDL_RenderCopy(renderer, blue_texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    SDL_Delay(500);
    

    SDL_Surface* Loading_Surf = SDL_LoadBMP("images/nuuhkis-modified.bmp");
    if (Loading_Surf == NULL) {
        cout << "EI ONNISTUNUT!" << endl;
    }
    auto nuuhkis = SDL_CreateTextureFromSurface(renderer, Loading_Surf);
    SDL_FreeSurface(Loading_Surf);
    
    
    int koko = 200;
    SDL_Rect dest{WIDTH/2 - koko/2, HEIGHT/2 - koko/2, koko, koko};
    SDL_RenderCopy(renderer, nuuhkis, nullptr, &dest);
    SDL_RenderPresent(renderer);
    //SDL_Delay(5000);
    
    
    bool running = true;
    double radius = 10;
    bool increase = false;
    bool decrease = false;
    int xpos;
    int ypos;
    int zoom = 0;
    bool zooming = false;
    bool downing = false;
    int shiftx = 0;
    int shifty = 0;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN) {
                //cout << "Painoit nappia" << endl;
                auto lol = e.key.keysym.sym;
                if (lol == SDLK_RIGHT) increase = true;
                if (lol == SDLK_LEFT) decrease = true;
                if (lol == SDLK_UP) zooming = true;
                if (lol == SDLK_DOWN) downing = true;
                /*
                switch(e.key.keysym.sym) {
                    case SDLK_RIGHT:
                        cout << "Oikea" << endl;
                    case SDLK_LEFT:
                        cout << "Vasen" << endl;
                }
                */
            }
            if (e.type == SDL_KEYUP) {
                auto lol = e.key.keysym.sym;
                if (lol == SDLK_RIGHT) increase = false;
                if (lol == SDLK_LEFT) decrease = false;
                if (lol == SDLK_UP) zooming = false;
                if (lol == SDLK_DOWN) downing = false;
            }
            if (e.type == SDL_MOUSEMOTION) {
                SDL_GetMouseState(&xpos, &ypos);
                xpos -= radius/2;
                ypos -= radius/2;
            }
        }
        if (increase) {
            radius += 1;
            shiftx++;
        }
        if (decrease) {
            radius -= 1;
            shiftx--;
        }
        if (zooming) zoom += 1;
        if (downing) zoom -= 1;
        
        draw_background(renderer);
        draw_rect(renderer, (int)radius, xpos, ypos);
        //draw_circle(renderer, radius);
        
        int w,h;
        SDL_QueryTexture(nuuhkis, nullptr, nullptr, &w, &h);
        SDL_Rect source{zoom+shiftx, zoom+shifty, w-2*zoom, h-2*zoom};
        SDL_RenderCopy(renderer, nuuhkis, &source, nullptr);
        SDL_SetRenderDrawColor(renderer, 0,0,0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, 0,0, WIDTH, HEIGHT);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
        //cout << "Tässä " << radius << endl;
    }

    SDL_Quit();

    return 0;
}
