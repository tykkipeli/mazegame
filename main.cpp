#include <iostream>
#include "Game.hpp"
#include <chrono>
// compile with g++ main.cpp -lSDL2 -lSDL2main -o main

using namespace std;

int main() {
    int SIZE = 100;
    int width = 17*SIZE;
    int height = 10*SIZE;
    int fps = 60;
    Game *game = new Game();
    game->init("Nuuh nuuuuh!", width, height, SIZE);
    
    std::chrono::steady_clock::time_point begin_time = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point prev_time = std::chrono::steady_clock::now(); 
    std::chrono::steady_clock::time_point mid_time = std::chrono::steady_clock::now();
    double update_time = 0;
    double render_time = 0;
    int round = 0;
    while (game->running()) {
        int start_time = SDL_GetTicks();
        game->handleEvents();
        
        prev_time = std::chrono::steady_clock::now();
        game->update();
        
        mid_time = std::chrono::steady_clock::now();
        update_time += std::chrono::duration_cast<std::chrono::microseconds>(mid_time - prev_time).count();
        prev_time = mid_time;
        
        game->render();
        mid_time = std::chrono::steady_clock::now();
        render_time += std::chrono::duration_cast<std::chrono::microseconds>(mid_time - prev_time).count();
        
        int diff = SDL_GetTicks() - start_time;
        if (1000/fps > diff) SDL_Delay(1000/fps - diff);
        
        //debugging stuff:
        round++;
        if (round%60 == 0) {
            end_time = std::chrono::steady_clock::now();
            //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - begin_time).count() << "[µs]" << std::endl;
            double time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - begin_time).count();
            cout << "Time for 60 frames: " << time/1000000 << " seconds" << endl;
            cout << "render_time: " << render_time/1000000 << "\nupdate_time: " << update_time/1000000 << endl;
            begin_time = end_time;
        }
    }

    game->clean();

    return 0;
}
