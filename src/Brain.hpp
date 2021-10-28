#ifndef Brain_hpp
#define Brain_hpp

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

const double G = 0.5;
const double SPRING_LENGTH = 1;
const double STIFFTNESS_COEFFITIENT = 2;
const double AIR_FRICTION = 0.99;
const double METER = 100;

enum Mode {edit, create, rmve};
struct Vector2D{
    double x;
    double y;
    double getLength(){
        return sqrt(x*x+y*y);
    }
};

class Brain{
public:
    static Vector2D addVectors(Vector2D _v1, Vector2D _v2){ return { _v1.x + _v2.x, _v1.y + _v2.y }; }
    static double getDistance(Vector2D _v1, Vector2D _v2){ return abs(sqrt((_v1.x-_v2.x)*(_v1.x-_v2.x)+(_v1.y-_v2.y)*(_v1.y-_v2.y))); }
    
    static void renderText(SDL_Renderer* _renderer, const char* _text, int _x, int _y, TTF_Font* _font);
    static void renderText(SDL_Renderer* _renderer, const char* _text, int _x, int _y, TTF_Font* _font, SDL_Color _color);
};

#endif /* Brain_hpp */
