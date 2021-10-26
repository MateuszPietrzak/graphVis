#ifndef Brain_hpp
#define Brain_hpp

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int G = 10;
const int MAX_DIST = 250;
const int SPRING_LENGTH = 100;
const float STIFFTNESS_COEFFITIENT = 10;
const double AIR_FRICTION = 0.99;

struct Vector2D{
    double x;
    double y;
    double getLength(){
        return sqrt(x*x+y*y);
    }
};

class Brain{
public:
    static Vector2D addVectors(Vector2D _v1, Vector2D _v2){
        return {
            _v1.x + _v2.x,
            _v1.y + _v2.y
        };
    }
    static double getDistance(Vector2D _v1, Vector2D _v2){
        return sqrt((_v1.x-_v2.x)*(_v1.x-_v2.x)+(_v1.y-_v2.y)*(_v1.y-_v2.y));
    }
};

#endif /* Brain_hpp */
