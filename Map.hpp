#pragma once
#include <vector>
#include <SDL2/SDL.h>

class Map {
public:
    Map();
    Map(double SIZE);
    
    void generate(int w, int h);
    // labyrintti_koordinaattien perusteella
    std::vector<std::pair<int,int>> get_neighbours(int x, int y);
    std::vector<std::pair<int,int>> get_connected_neighbours(int x, int y);
    
    
    void update(double player_x, double player_y, int screenwidth, int screenheight);
    void draw(SDL_Renderer *renderer, double player_x, double player_y, int screenwidth, int screenheight);
    //todellisen koordinaatin perusteella
    std::vector<SDL_FRect> get_close_walls(double x, double y, int radius);
    std::pair<double, double> get_random_position(double radius, int x, int y);
    
    
    std::vector<std::vector<std::vector<std::pair<int,int>>>> maze;
    std::vector<std::vector<std::vector<std::pair<std::pair<double,double>, std::pair<double,double>>>>> wall_segments;
    
    int w;
    int h;
    double wall_thickness;
    double SIZE;
    
    // unnecessary:
    std::vector<std::pair<std::pair<double,double>, std::pair<double,double>>> relevant_wall_segments;
    
private:
    std::vector<std::vector<int>> visited;
    
    void dfs(int x, int y);
};
