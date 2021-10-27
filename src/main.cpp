#include <iostream>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Vertex.hpp"
#include "Brain.hpp"

int main() {
    //std::cout << cos(1.5708) << " " << cos(1.5708+M_PI) << std::endl;

    TTF_Font* font1 = NULL;

    bool running = true;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    
    std::vector<Vertex*> ver;
    std::map<int,bool> keyboard;
    
    const int FPS = 60;
    const int frameDelay = 1000/FPS;
    Uint32 frameStart;
    int frameTime;

    int selected = 0;
    int mx, my;
    bool lmbd = false, rmbd = false;
    
    if(SDL_Init(SDL_INIT_EVERYTHING)){
        std::cout << "Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    window = SDL_CreateWindow("Graph", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
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
    
    if(TTF_Init()){
        std::cout << "Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    font1 = TTF_OpenFont("assets/font1.ttf", 16);

    for(int i = 0; i<6; ++i){
        ver.push_back(new Vertex(i+1,250+(i%2)*100,100+((i-(i%2))*100),1,renderer));
    }

    ver[0]->addConnection(2);
    ver[1]->addConnection(1);
    ver[2]->addConnection(1);
    ver[0]->addConnection(3);
    ver[3]->addConnection(1);
    ver[0]->addConnection(4);
    ver[4]->addConnection(2);
    ver[1]->addConnection(5);
    ver[5]->addConnection(2);
    ver[1]->addConnection(6);
    ver[2]->addConnection(5);
    ver[4]->addConnection(3);

    while(running){
        frameStart = SDL_GetTicks();
        SDL_GetMouseState(&mx, &my);
        while(SDL_PollEvent(&event)){
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.button == SDL_BUTTON_LEFT){
                        if(lmbd) continue;
                        lmbd = true;
                        for(auto& i : ver){
                            if(Brain::getDistance(i->getPos(),{(double)mx,(double)my}) < 7){
                                if(!selected){
                                    selected = i->getId();
                                }else if(selected == i->getId()){
                                    selected = 0;
                                }else{
                                    if(ver[i->getId()-1]->isConnected(selected)){
                                        ver[i->getId()-1]->removeConnection(selected);
                                        ver[selected-1]->removeConnection(i->getId());
                                    }else{
                                        ver[i->getId()-1]->addConnection(selected);
                                        ver[selected-1]->addConnection(i->getId());
                                    }
                                    
                                    selected = 0;
                                }
                            }
                        }
                    }else{
                        if(rmbd) continue;
                        rmbd = true;
                        selected = 0;
                    }
                    
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(event.button.button == SDL_BUTTON_LEFT){
                        lmbd = false;
                    }else{
                        rmbd = false;
                    }
                    break;

                case SDL_KEYDOWN:
                    keyboard[event.key.keysym.sym] = false;
                    break;
                case SDL_KEYUP:
                    keyboard[event.key.keysym.sym] = true;
                    break;

                default:
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        
        for(auto& i: ver){
            //std::cout << i->getPos().x << " " << i->getPos().y << std::endl;
            if(i->getId() == selected){
                i->recalculateForce({((double)mx-i->getPos().x)/1000,((double)my-i->getPos().y)/1000});
            }
            for(auto& j:ver){
                if(i->getId() == j->getId()/* || i->getId() == 1*/) continue;
                if(!i->isConnected(j->getId())){
                    double temp_r = Brain::getDistance(i->getPos(),j->getPos()) / METER;
                    double temp_v_mag = - G * (
                        (i->getMass() * j->getMass()) /
                        (temp_r * temp_r));
                    double temp_v_angle = atan2(
                        j->getPos().y - i->getPos().y,
                        j->getPos().x - i->getPos().x);
                    i->recalculateForce({
                        temp_v_mag * cos(temp_v_angle),
                        temp_v_mag * sin(temp_v_angle)
                    });
                    //std::cout << (temp_v_angle) << " " << temp_v_mag * cos(temp_v_angle) << " " << temp_v_mag * sin(temp_v_angle) << std::endl;
                }
                else{
                    double temp_r = Brain::getDistance(i->getPos(),j->getPos()) / METER;
                    double temp_v_mag = (temp_r - SPRING_LENGTH) * STIFFTNESS_COEFFITIENT;
                    double temp_v_angle = atan2(
                        j->getPos().y - i->getPos().y,
                        j->getPos().x - i->getPos().x);
                    i->recalculateForce({
                        temp_v_mag * cos(temp_v_angle),
                        temp_v_mag * sin(temp_v_angle)
                    });
                    //std::cout << (temp_v_angle) << " " << temp_v_mag * cos(temp_v_angle) << " " << temp_v_mag * sin(temp_v_angle) << std::endl;
                }
                
            }
            //i->recalculateForce({1,1});
            
            
            
        }
        for(auto& i: ver){
            i->update();
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            for(auto& j: ver){
                
                if(i->isConnected(j->getId())){
                    SDL_RenderDrawLine(renderer, i->getPos().x,i->getPos().y, j->getPos().x,j->getPos().y);
                }
            }
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            
        }
        for(auto& i: ver){
            if(i->getId() == selected){
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            }else{
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            }
            i->render();
        }
        if(selected != 0) Brain::renderText(renderer, std::to_string(ver[selected-1]->getId()).c_str(), 5, 5, font1);
        //std::cout << std::endl;
        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    }
    
    TTF_CloseFont(font1);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
