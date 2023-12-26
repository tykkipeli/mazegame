#include "Monster.hpp"
#include <iostream>
#include "utils.hpp"
#include "physics.hpp"
#include <stdlib.h>

using CIRCLE_LIST = std::vector<std::pair<std::pair<double,double>,double>>;
using PDD = std::pair<double,double>;

Monster::Monster()
{}


Monster::Monster(double x, double y, double radius) {
    this->x = x;
    this->y = y;
    this->target_x = x;
    this->target_y = y;
    this->radius = radius;
    this->speed = 1;
    this->prev_x = -1;
    this->prev_y = -1;
    this->angle = 0;
    this->target_angle = 0;
    this->angle_speed = 0.005;
}

void Monster::load_image(SDL_Renderer *renderer) {
    this->image_texture = load_texture("images/makkis-modified.bmp", renderer);
}

void Monster::draw(SDL_Renderer *renderer, double player_x, double player_y, int screenwidth, int screenheight) {
    float offsetx = screenwidth/2.0 - player_x;
    float offsety = screenheight/2.0 - player_y; 
    SDL_FRect dest{(float) (offsetx + x - radius), (float) (offsety + y - radius), (float) (2*radius), (float) (2*radius)};
    SDL_RenderCopyExF(renderer, this->image_texture, nullptr, &dest, angle*(180.0/PII) - 96, nullptr, SDL_FLIP_NONE);
}

void Monster::draw_rays(SDL_Renderer *renderer, double player_x, double player_y, int screenwidth, int screenheight) {
    float offsetx = screenwidth/2.0 - player_x;
    float offsety = screenheight/2.0 - player_y;
    //SDL_SetRenderDrawColor(renderer, 255, 127, 0, 100);
    SDL_SetRenderDrawColor(renderer, 255, 252, 161, 100);
    for (auto ray : this->rays) {
        float x1 = std::min((double)screenwidth, std::max(0.0, offsetx + ray.first.first));
        float y1 = std::min((double)screenheight, std::max(0.0, offsety + ray.first.second));
        float x2 = std::min((double)screenwidth, std::max(0.0, offsetx + ray.second.first));
        float y2 = std::min((double)screenheight, std::max(0.0, offsety + ray.second.second));
        x1 = offsetx + ray.first.first;
        y1 = offsety + ray.first.second;
        x2 = offsetx + ray.second.first;
        y2 = offsety + ray.second.second;
        SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
        //SDL_RenderDrawLineF(renderer, x1, y1, x2+1, y2+1);
        //SDL_RenderDrawLineF(renderer, x1, y1, x2-1, y2-1);
    }
}

void Monster::update(Map &map) {
    //std::cout << "updating a monster" << std::endl;
    for (int i = 0; i < speed; i++) {
        if (std::abs(angle - target_angle) <= 1.5*angle_speed) {
            angle = target_angle;
            this->move(map);
        } else {
            angle = towards_angle(angle, target_angle, angle_speed);
        }
    }
    // calcualte rays and store them in vector
    // only if close to player
}

void Monster::move(Map &map) {
    double dist = (x - target_x)*(x-target_x) + (y-target_y)*(y-target_y);
    if (dist <= 2) {
        this->change_target(map);
    }
    this->move_x(1);
    this->move_y(1);
    //double dist2 = (x - target_x)*(x-target_x) + (y-target_y)*(y-target_y);
    //if (dist2 > dist) angle += PII;
}
void Monster::change_target(Map &map) {
    double SIZE = map.SIZE;
    double gap = 3*SIZE;
    int i = x/gap;
    int j = y/gap;
    auto neighbours = map.get_connected_neighbours(i,j);
    std::vector<std::pair<int,int>> uudet;
    for (auto p : neighbours) {
        if (p.first != prev_x || p.second != prev_y) uudet.push_back(p);
    }
    std::pair<int,int> new_p = {prev_x, prev_y};
    if ((uudet.size() > 0 && rand()%16 > 0) || (prev_x == -1 && prev_y == -1)) {
        new_p = choice(uudet);
    }
    prev_x = i;
    prev_y = j;
    auto target = map.get_random_position(radius, new_p.first, new_p.second);
    target_x = target.first;
    target_y = target.second;
    double dx = target_x - this->x;
    double dy = target_y - this->y;
    target_angle = std::atan2(dy, dx); // angle between [-PII, PII]
    //angle = std::atan2(dy, dx);
}



// TODO Optimointi idea:
// TODO Jos säde ei leikkaa pellajan näyttöä vastaavaa suorakulmiota, ei tarvitse tehdä mitään
void Monster::update_rays(Map &map, CIRCLE_LIST &circles) {
    this->rays.clear();
    double monta = 60;
    double ray_length = 3*map.SIZE;
    double gap = ray_length;
    for (int i = 0; i < monta; i++) {
        double ray_angle = angle + (monta/2 - i)/monta;
        double dx = std::cos(ray_angle);
        double dy = std::sin(ray_angle);
        PDD a = {x,y};
        PDD b = {x+dx*ray_length, y+dy*ray_length};
        PDD NONE = {-1e9, -1e9};
        PDD collision_point = NONE;
        
        //line intersection
        while (collision_point == NONE) {
            
            // leikkaako [a,b] jonkun seinä segmentin a:n ruudussa:
            int I = a.first/gap;
            int J = a.second/gap;
            collision_point = closest_intersection(a,b, map.wall_segments[I][J]);
            if (collision_point != NONE) {
                break;
            }
            
            // leikaako [a,b] jonkun seinä segmentin b:n ruudussa:
            I = b.first/gap;
            J = b.second/gap;
            I = std::max(0, I);
            I = std::min(map.w - 1, I);
            J = std::max(0, J);
            J = std::min(map.h - 1, J);
            collision_point = closest_intersection(a,b, map.wall_segments[I][J]);
            
            a = b;
            b = {b.first + dx*ray_length, b.second + dy*ray_length};
        }
        // circle_intersections:
        a = {x,y};
        b = {x + dx*ray_length*100, y + dy*ray_length*100};
        //PDD closes_circle = {-1,-1};
        //double circ_dist = 1e9;
        double dist = distance(a, collision_point);
        for (auto circle : circles) {
            PDD point = circle_segment_intersection(a, b, circle.first, circle.second);
            double dist2 = distance(a, point);
            if (std::abs(dist2 - radius*radius) < 0.01) continue;
            if (dist2 < dist) {
                dist = dist2;
                collision_point = point;
            }
        }
        this->rays.push_back({{x,y},collision_point});
    }
}


/*
 def get_rays(self, player):
        rays = []
        ray_length = 6*self.SIZE
        for monster in self.monsters:
            dist_squared = (monster.x-player.x)**2 + (monster.y-player.y)**2
            if dist_squared > (6*self.SIZE)**2:
                continue
            monta = 30
            for i in range(monta):
                angle = monster.angle + (monta/2-i)/monta
                dx = cos(angle)
                dy = sin(angle)
                collision_point = ray_collision((monster.x, monster.y), (monster.x+dx*ray_length, monster.y+dy*ray_length), self)
                rays.append(((monster.x, monster.y), collision_point))
        return rays

*/

void Monster::move_x(double k) {
    double dx = std::cos(angle);
    this->x += k*dx;
}

void Monster::move_y(double k) {
    double dy = std::sin(angle);
    this->y += k*dy;
}
