#include "Vertex.hpp"

Vertex::Vertex(int _id, double _x, double _y, double _mass, SDL_Renderer* _renderer)
{
    force = {0,0};
    renderer = _renderer;
    
    pos.x = _x;
    pos.y = _y;
    mass = _mass;
    acceleration = 0;
    velocity = 0;
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
    //std::cout << "{ " << force.x << " " << force.y << " }" << std::endl;
    force_angle = atan2(force.y, force.x);
    acceleration = force.getLength() / mass;
    //std::cout << force.x << " " << force.y  << " ";
    //std::cout << acceleration << " ";
    
    velocity += acceleration;
    //std::cout << velocity << " ";
    pos.x += velocity * cos(force_angle);
    pos.y += velocity * sin(force_angle);
    velocity *= AIR_FRICTION;
    
    SDL_RenderDrawLine(renderer, pos.x, pos.y, pos.x + 100000 * force.getLength() * cos(force_angle), pos.y + 100000 * force.getLength() * sin(force_angle));
    
    force = {0,0};
    //std::cout << pos.x << " " << pos.y << " ";
}

void Vertex::render()
{
    //std::cout << pos.x << " " << pos.y << std::endl;
    SDL_Rect tempRect = {(int)pos.x-5,(int)pos.y-5,10,10};
    SDL_RenderFillRect(renderer, &tempRect);
}
