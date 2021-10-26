#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Vertex.hpp"
#include "Brain.hpp"

int main() {
    bool running = true;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    SDL_Event event;
    
    std::vector<Vertex*> ver;
    
    const int FPS = 60;
    const int frameDelay = 1000/FPS;
    Uint32 frameStart;
    int frameTime;
    
    if(SDL_Init(SDL_INIT_EVERYTHING)){
        std::cout << "Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    window = SDL_CreateWindow("Graph", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000, 0);
    if(window == NULL){
        SDL_Quit();
        std::cout << "Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(window == NULL){
        SDL_DestroyWindow(window);
        SDL_Quit();
        std::cout << "Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    ver.push_back(new Vertex(1,500,500,renderer));
    ver.push_back(new Vertex(2,500,550,renderer));
    ver.push_back(new Vertex(3,550,550,renderer));
    ver.push_back(new Vertex(4,450,550,renderer));
    ver[0]->addConnection(2);
    ver[1]->addConnection(1);

    while(running){
        frameStart = SDL_GetTicks();
        while(SDL_PollEvent(&event)){
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                    
                default:
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        
        for(auto& i: ver){
            std::cout << i->getPos().x << " " << i->getPos().y << std::endl;
            for(auto& j:ver){
                if(i->getId() != j->getId()){
                    if(!i->isConnected(j->getId())){
                        double temp_r = Brain::getDistance(i->getPos(), j->getPos());
                        if(temp_r > MAX_DIST) continue;
                        double temp_force_val = G*(i->getMass()*-j->getMass())/(temp_r == 0 ? 0.0001 : temp_r*temp_r);
                        double temp_force_angle = atan2(i->getPos().y-j->getPos().y,j->getPos().x-i->getPos().x);
                        i->recalculateForce({temp_force_val*cos(temp_force_angle),-temp_force_val*sin(temp_force_angle)});
                    }else{
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        SDL_RenderDrawLine(renderer, i->getPos().x, i->getPos().y, j->getPos().x, j->getPos().y);
                        
                        double temp_r = abs(Brain::getDistance(i->getPos(), j->getPos()));
                        double x = temp_r-SPRING_LENGTH;
                        double temp_force_angle = atan2(i->getPos().y-j->getPos().y,j->getPos().x-i->getPos().x);
                        i->recalculateForce({0.001*x*cos(temp_force_angle),-0.001*x*sin(temp_force_angle)});
                    }
                }
            }
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            i->update();
            
        }
        for(auto& i: ver){
            i->render();
        }
        //std::cout << std::endl;
        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
