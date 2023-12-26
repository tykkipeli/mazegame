#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include "Map.hpp"
#include "utils.hpp"
#include <algorithm>

Map::Map()
{}

Map::Map(double SIZE) {
    this->SIZE = SIZE;
}

void Map::generate(int w, int h) {
    this->w = w;
    this->h = h;
    this->wall_thickness = SIZE/4;
    this->wall_thickness *= 4;
    std::cout << "generating map" << std::endl;
    std::vector<std::vector<std::vector<std::pair<int,int>>>> new_maze(w,std::vector<std::vector<std::pair<int,int>>>(h));
    std::vector<std::vector<int>> new_visited(w,std::vector<int>(h,0));
    swap(this->maze, new_maze);
    swap(this->visited, new_visited);
    this->dfs(0,0);
    this->wall_segments.clear();
    for (int i = 0; i < w; i++) {
        std::vector<std::vector<std::pair<std::pair<double,double>, std::pair<double,double>>>> row;
        for (int j = 0; j < h; j++) {
            std::vector<std::pair<std::pair<double,double>, std::pair<double,double>>> column;
            auto walls = this->get_close_walls(i*3*SIZE,j*3*SIZE, 0);
            for (auto wall : walls) {
                //ylä
                column.push_back({{wall.x, wall.y},{wall.x+wall.w, wall.y}});
                //ala
                column.push_back({{wall.x, wall.y+wall.h},{wall.x+wall.w, wall.y+wall.h}});
                //oikea
                column.push_back({{wall.x+wall.w, wall.y},{wall.x+wall.w, wall.y+wall.h}});
                //vasen
                column.push_back({{wall.x, wall.y},{wall.x, wall.y+wall.h}});
            }
            row.emplace_back(column);
        }
        this->wall_segments.emplace_back(row);
    }
    std::cout << "done" << std::endl;
}


void Map::dfs(int x, int y) {
    if (this->visited[x][y]) return;
    this->visited[x][y] = true;
    auto neighbours = this->get_neighbours(x,y);
    shuffle(neighbours);
    for (auto p : neighbours) {
        if (this->visited[p.first][p.second]) continue;
        auto p_neighbours = this->get_neighbours(p.first, p.second);
        std::vector<std::pair<int,int>> vis_neighbours;
        for (auto pp : p_neighbours) {
            if (this->visited[pp.first][pp.second]) vis_neighbours.push_back(pp);
        }
        auto p2 = choice(vis_neighbours);
        this->maze[p2.first][p2.second].push_back(p);
        this->maze[p.first][p.second].push_back(p2);
        this->dfs(p.first, p.second);
        
    } 
}

void Map::update(double player_x, double player_y, int screenwidth, int screenheight) {
    /*
    this->relevant_wall_segments.clear();
    auto walls = this->get_close_walls(player_x, player_y, 6);
    //std::vector<std::pair<std::pair<double,double>, std::pair<double,double>>> relevant_wall_segments;
    std::vector<std::pair<std::pair<double,double>, std::pair<double,double>>> temp;
    for (auto wall : walls) {
        //ylä
        temp.push_back({{wall.x, wall.y},{wall.x+wall.w, wall.y}});
        //ala
        temp.push_back({{wall.x, wall.y+wall.h},{wall.x+wall.w, wall.y+wall.h}});
        //oikea
        temp.push_back({{wall.x+wall.w, wall.y},{wall.x+wall.w, wall.y+wall.h}});
        //vasen
        temp.push_back({{wall.x, wall.y},{wall.x, wall.y+wall.h}});
    }
    sort(temp.begin(), temp.end());
    for (int i = 0; i < temp.size(); i++) {
        if (i > 0 && temp[i] == temp[i-1]) continue;
        auto p = temp[i];
        relevant_wall_segments.push_back(p);
    }
    */
}

std::vector<std::pair<int,int>> Map::get_neighbours(int x, int y) {
    std::vector<std::pair<int,int>> ans;
    std::vector<std::pair<int,int>> dirs = {{x,y-1}, {x,y+1}, {x+1,y}, {x-1,y}};
    for (int i = 0; i < 4; i++) {
        auto p = dirs[i];
        if (p.first >= 0 && p.first < this->w && p.second >= 0 && p.second < this->h) {
            ans.push_back(p);
        }
    }
    return ans; 
}

std::vector<std::pair<int,int>> Map::get_connected_neighbours(int x, int y) {
    std::vector<std::pair<int,int>> ans;
    auto all = this->get_neighbours(x, y);
    for (auto p : all) {
        if (contains(this->maze[x][y], p)) ans.push_back(p);
    }
    return ans; 
}

std::pair<double, double> Map::get_random_position(double radius, int x, int y) {
    double gap = 3*SIZE;
    double padding = radius + this->wall_thickness/2;
    double ilisa = padding + rand()%((int)(gap-2*padding));
    double jlisa = padding + rand()%((int)(gap-2*padding));
    return {gap*x + ilisa, gap*y + jlisa};
}


void draw_rect(SDL_Renderer *renderer, float x, float y, float width, float height) {
    SDL_FRect r = {x, y, width, height};
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRectF(renderer, &r);
}

void Map::draw(SDL_Renderer *renderer, double x, double y, int screenwidth, int screenheight) {
    double gap = 3*SIZE;
    double offsetx = screenwidth/2 - x;
    double offsety = screenheight/2 - y;
    int starti = x/gap - 3;
    int startj = y/gap - 3;
    for (int i = std::max(0,starti); i < std::min(this->w, starti+7); i++) {
        for (int j = std::max(0,startj); j < std::min(this->h, startj+7); j++) {
            //vasen reuna (i-1, j)
            if (!contains(this->maze[i][j], {i-1,j})) {
                draw_rect(renderer, offsetx + i*gap - this->wall_thickness/2.0, offsety + j*gap, this->wall_thickness, gap);
            }
            //oikea reuna (i+1, j)
            if (!contains(this->maze[i][j], {i+1,j})) {
                draw_rect(renderer, offsetx + (i+1)*gap - this->wall_thickness/2.0, offsety + j*gap, this->wall_thickness, gap);
            }
            //ylä reuna (i, j-1)
            if (!contains(this->maze[i][j], {i,j-1})) {
                draw_rect(renderer, offsetx + i*gap, offsety + j*gap - this->wall_thickness/2.0, gap, this->wall_thickness);
            }
            //ala reuna (i, j+1)
            if (!contains(this->maze[i][j], {i,j+1})) {
                draw_rect(renderer, offsetx + i*gap, offsety + (j+1)*gap - this->wall_thickness/2.0, gap, this->wall_thickness);
            }
        }
    }
}

std::vector<SDL_FRect> Map::get_close_walls(double x, double y, int radius) {
    std::vector<SDL_FRect> ans;
    float gap = 3*SIZE;
    int mid_i = (int) (x/gap);
    int mid_j = (int) (y/gap);
    for (int i = mid_i - radius; i <= mid_i + radius; i++) {
        for (int j = mid_j - radius; j <= mid_j + radius; j++) {
            if (i < 0 || j < 0 || i >= this->w || j >= this->h) continue;
            // lisää ansiin ruudun (i,j) ympäröimät seinät
            //vasen reuna (i-1, j)
            if (!contains(this->maze[i][j], {i-1,j})) {
                ans.push_back({(float) (i*gap - this->wall_thickness/2.0), (float) j*gap, (float)this->wall_thickness, gap});
            }
            //oikea reuna (i+1, j)
            if (!contains(this->maze[i][j], {i+1,j})) {
                ans.push_back({(float) ((i+1)*gap - this->wall_thickness/2.0), (float) j*gap, (float)this->wall_thickness, gap});
            }
            //ylä reuna (i, j-1)
            if (!contains(this->maze[i][j], {i,j-1})) {
                ans.push_back({(float) i*gap, (float) (j*gap - this->wall_thickness/2.0), gap, (float)this->wall_thickness});
            }
            //ala reuna (i, j+1)
            if (!contains(this->maze[i][j], {i,j+1})) {
                ans.push_back({(float) i*gap , (float) ((j+1)*gap - this->wall_thickness/2.0), gap, (float)this->wall_thickness});
            }
        }
    }
    return ans;
}







