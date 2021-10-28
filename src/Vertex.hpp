#ifndef Vertex_hpp
#define Vertex_hpp

#include <iostream>
#include <set>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Brain.hpp"

class Vertex{
public:
    Vertex(int _id, double _x, double _y, double _mass, SDL_Renderer* _renderer);
    ~Vertex();
    
    void recalculateForce(Vector2D _force);
    void update();
    void render(TTF_Font* _font);
    
    //Setters
    
    void addConnection(int _v){connections.insert(_v);}
    void removeConnection(int _v){connections.erase(_v);}
    void setId(int _v){id = _v;}
    void setPinned(){pinned = true;}
    void setUnpinned(){pinned = false;}

    //Getters
    
    Vector2D getPos(){return pos;}
    int getMass(){return mass;}
    int getId(){return id;}
    bool isConnected(int _v){
        auto i = connections.lower_bound(_v);
        if(i == connections.end()) return false;
        if(*i != _v) return false;
        return true;
    }
    Vector2D getVelocity(){return {velocity_x,velocity_y};}
    bool isPinned(){return pinned;}
    
private:
    SDL_Renderer* renderer;
    
    int id;
    
    Vector2D pos;
    Vector2D force;
    double force_angle;
    double acceleration_x, acceleration_y;
    double velocity_x, velocity_y;
    double mass;
    std::set<int> connections;

    bool pinned;
};

#endif /* Vertex_hpp */
