#include "Vertex.hpp"

Vertex::Vertex(int _id, double _x, double _y, double _mass, SDL_Renderer* _renderer)
{
    force = {0,0};
    renderer = _renderer;
    
    pos.x = _x;
    pos.y = _y;
    mass = _mass;
    acceleration_x = 0;
    acceleration_y = 0;
    velocity_x = 0;
    velocity_y = 0;
    connections.clear();
    
    id = _id;
}

Vertex::~Vertex()
{
    
}

void Vertex::recalculateForce(Vector2D _force)
{
    force = Brain::addVectors(force, _force);
}

void Vertex::update()
{
    force_angle = atan2(force.y, force.x);

    acceleration_x = force.getLength() / mass * cos(force_angle);
    acceleration_y = force.getLength() / mass * sin(force_angle);

    velocity_x += acceleration_x;
    velocity_y += acceleration_y;

    pos.x += velocity_x * (double)60/1000;
    pos.y += velocity_y * (double)60/1000;
    
    velocity_x *= AIR_FRICTION;
    velocity_y *= AIR_FRICTION;
    
    SDL_RenderDrawLine(renderer, pos.x, pos.y, pos.x + 100 * force.getLength() * cos(force_angle), pos.y + 100 * force.getLength() * sin(force_angle));
    
    force = {0,0};
}

void Vertex::render()
{
    //std::cout << pos.x << " " << pos.y << std::endl;
    SDL_Rect tempRect = {(int)pos.x-5,(int)pos.y-5,10,10};
    SDL_RenderFillRect(renderer, &tempRect);
}
