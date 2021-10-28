#include "Brain.hpp"

void Brain::renderText(SDL_Renderer* _renderer, const char* _text, int _x, int _y, TTF_Font* _font)
{
    SDL_Surface* temp_surface = TTF_RenderText_Solid(_font, _text, {0,0,0,0});
    SDL_Texture* temp_texture = SDL_CreateTextureFromSurface(_renderer, temp_surface);
    SDL_Rect temp_rect = {_x,_y,temp_surface->w,temp_surface->h};
    SDL_FreeSurface(temp_surface);
    SDL_RenderCopy(_renderer, temp_texture, NULL, &temp_rect);
    SDL_DestroyTexture(temp_texture);
}

void Brain::renderText(SDL_Renderer* _renderer, const char* _text, int _x, int _y, TTF_Font* _font, SDL_Color _color)
{
    SDL_Surface* temp_surface = TTF_RenderText_Solid(_font, _text, _color);
    SDL_Texture* temp_texture = SDL_CreateTextureFromSurface(_renderer, temp_surface);
    SDL_Rect temp_rect = {_x,_y,temp_surface->w,temp_surface->h};
    SDL_FreeSurface(temp_surface);
    SDL_RenderCopy(_renderer, temp_texture, NULL, &temp_rect);
    SDL_DestroyTexture(temp_texture);
}