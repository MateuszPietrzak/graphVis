#ifndef Vertex_hpp
#define Vertex_hpp

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Brain.hpp"

class Vertex{
public:
    Vertex(int _id, double _x, double _y, SDL_Renderer* _renderer);
    ~Vertex();
    
    void recalculateForce(Vector2D _force);
    void update();
    void render();
    
    //Setters
    
    void addConnection(int _v){connections.push_back(_v);}
    
    //Getters
    
    Vector2D getPos(){return pos;}
    int getMass(){return mass;}
    int getId(){return id;}
    bool isConnected(int _v){
        for(auto& i : connections) if(i == _v) return true;
        return false;
    }
    
private:
    SDL_Renderer* renderer;
    
    int id;
    
    Vector2D pos;
    Vector2D force;
    double force_angle;
    double acceleration;
    double velocity;
    double mass;
    std::vector<int> connections;
};

#endif /* Vertex_hpp */
