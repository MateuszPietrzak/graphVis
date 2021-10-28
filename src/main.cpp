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

    std::vector<int> selected;
    int mx, my;
    bool lmbd = false, rmbd = false;
    bool shiftd = false;

    bool simulating = false;

    Mode mode = edit;

    int l = 1;
    bool any_pinned;
    
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

    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );

    /*for(int i = 0; i<6; ++i){
        ver.push_back(new Vertex(l,250+(i%2)*100,100+((i-(i%2))*100),1,renderer));
        l++;
    }*/

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
                        if(mode == edit){
                            if(!shiftd){
                                for(auto& i : ver){
                                    if(Brain::getDistance(i->getPos(),{(double)mx,(double)my}) < 7){
                                        if(selected.empty()){
                                            selected.push_back(i->getId());
                                        }else{
                                            for(auto& sel : selected){
                                                if(ver[i->getId()-1]->isConnected(sel)){
                                                    ver[i->getId()-1]->removeConnection(sel);
                                                    ver[sel-1]->removeConnection(i->getId());
                                                }else{
                                                    ver[i->getId()-1]->addConnection(sel);
                                                    ver[sel-1]->addConnection(i->getId());
                                                }
                                                
                                                sel = i->getId();
                                            }
                                        }
                                    }
                                }
                                selected.clear();
                            }else{
                                
                                for(int i = 0 ; i<ver.size(); ++i){
                                    
                                    if(Brain::getDistance(ver[i]->getPos(),{(double)mx,(double)my}) < 7){
                                        bool in = false;
                                        for(auto& j: selected){
                                            if(j == ver[i]->getId()) in = true;
                                        }
                                        if(!in) selected.push_back(ver[i]->getId());
                                        else selected.erase(selected.begin()+i);
                                    }
                                }
                            }
                        }
                        if(mode == create){
                            bool touchingsth = false;
                            for(auto& i : ver){
                                if(Brain::getDistance(i->getPos(),{(double)mx,(double)my}) < 7){
                                    touchingsth = true;
                                }
                            }
                            if(!touchingsth){
                                ver.push_back(new Vertex(ver.size()+1,mx,my,1,renderer));
                            }
                        }
                        if(mode == rmve){
                            int rid = 0;
                            for(auto& i : ver){
                                if(Brain::getDistance(i->getPos(),{(double)mx,(double)my}) < 7){
                                    rid = i->getId();
                                }
                            }
                            if(rid){
                                bool before = true;
                                for(int i = 1; i<=ver.size(); ++i){
                                    if(ver[i-1]->isConnected(rid)){
                                        ver[i-1]->removeConnection(rid);
                                        //std::cout << "removing " << rid << " from " << i << std::endl;
                                    } 
                                    if(!before) {
                                        for(int j = 1; j<=ver.size(); ++j){
                                            if(ver[j-1]->isConnected(i)){
                                                ver[j-1]->removeConnection(i);
                                                ver[j-1]->addConnection(i-1);
                                            }
                                        }
                                        ver[i-1]->setId(ver[i-1]->getId()-1);
                                    }
                                    if(i == rid){
                                        before = false;
                                    }
                                }
                                ver.erase(ver.begin()+rid-1);
                            }
                        }
                    }
                    if(event.button.button == SDL_BUTTON_RIGHT){
                        if(rmbd) continue;
                        rmbd = true;
                        selected.clear();
                    }
                    
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(event.button.button == SDL_BUTTON_LEFT){
                        lmbd = false;
                    }
                    if(event.button.button == SDL_BUTTON_RIGHT){
                        rmbd = false;
                    }
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            simulating = !simulating;
                            break;
                        case SDLK_LSHIFT:
                            shiftd = true;
                            break;
                        case SDLK_1:
                            selected .clear();
                            mode = edit;
                            break;
                        case SDLK_2:
                            selected .clear();
                            mode = create;
                            break;
                        case SDLK_3:
                            selected .clear();
                            mode = rmve;
                            break;
                        case SDLK_l:
                            for(int i = 0; i<ver.size(); ++i){
                                std::cout << ver[i]->getId() << ": ";
                                for(int j = 0; j<ver.size(); ++j){
                                    if(ver[i]->isConnected(j+1)) std::cout << "{ " << j+1 << " " << ver[j]->getId() << " } ";
                                }
                                std::cout << std::endl;
                            }
                            break;
                        case SDLK_p:
                            any_pinned = false;
                            for(auto& sel : selected){
                                if(ver[sel-1]->isPinned()) any_pinned = true;
                            }
                            for(auto& sel : selected){
                                if(!sel) break;
                                any_pinned ? ver[sel-1]->setUnpinned() : ver[sel-1]->setPinned();
                            }
                            selected.clear();
                            break;
                        case SDLK_r:
                            ver.clear();
                            selected.clear();
                            break;
                        default:
                            break;
                    }
                    break;

                case SDL_KEYUP:
                    switch(event.key.keysym.sym){
                        case SDLK_LSHIFT:
                            shiftd = false;
                            break;
                        default:
                            break;
                    }


                default:
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        
        for(auto& i: ver){
            //std::cout << i->getPos().x << " " << i->getPos().y << std::endl;
            bool inselected = false;
            for(auto& j : selected){
                if(i->getId() == j) inselected = true;
            }

            if(inselected && simulating){
                i->recalculateForce({((double)mx-i->getPos().x)/300,((double)my-i->getPos().y)/300});
            }
            for(auto& j:ver){
                if(i->getId() == j->getId()) continue;
                if(!i->isConnected(j->getId())){
                    double temp_r = Brain::getDistance(i->getPos(),j->getPos()) / METER;
                    double temp_v_mag = - G * (
                        (i->getMass() * j->getMass()) /
                        (temp_r * temp_r));
                    double temp_v_angle = atan2(
                        j->getPos().y - i->getPos().y,
                        j->getPos().x - i->getPos().x);
                    if(simulating) i->recalculateForce({
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
                    if(simulating) i->recalculateForce({
                        temp_v_mag * cos(temp_v_angle),
                        temp_v_mag * sin(temp_v_angle)
                    });
                    //std::cout << (temp_v_angle) << " " << temp_v_mag * cos(temp_v_angle) << " " << temp_v_mag * sin(temp_v_angle) << std::endl;
                }
                
            }
            //i->recalculateForce({1,1});
            
            
            
        }
        for(auto& i: ver){
            if(simulating) i->update();
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            for(auto& j: ver){
                
                if(i->isConnected(j->getId())){
                    SDL_RenderDrawLine(renderer, i->getPos().x,i->getPos().y, j->getPos().x,j->getPos().y);
                }
            }
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            
        }

        for(auto& i: ver){
            bool inselected = false;
            for(auto& j : selected){
                if(i->getId() == j) inselected = true;
            }
            SDL_SetRenderDrawColor(renderer, 255, i->isPinned() ? 255 : 0, inselected ? 255 : 0 , 255);
            i->render(font1);
        }


        //Brain::renderText(renderer, std::to_string(ver[selected-1]->getId()).c_str(), 5, 5, font1);
        Brain::renderText(renderer, "[p] pin / unpin",5,21,font1);
        Brain::renderText(renderer, simulating ? "[Esc] pause" : "[Esc] resume", 650,5,font1, simulating ? SDL_Color({0,255,0,255}) : SDL_Color({255,0,0,255}));
        Brain::renderText(renderer, "[r] reset",650,21,font1);
        
        Brain::renderText(renderer, mode == edit ? "-> [1] edit mode" : "[1] edit mode", 5, 547, font1);
        Brain::renderText(renderer, mode == create ? "-> [2] create mode" : "[2] create mode", 5, 563, font1);
        Brain::renderText(renderer, mode == rmve ? "-> [3] delete mode" : "[3] delete mode", 5, 579, font1);

        if(shiftd) Brain::renderText(renderer, "Shift", 650, 584,font1);

        /*for(int i = 0; i<ver.size(); ++i){
            Brain::renderText(renderer, std::to_string(i).c_str(), 750,400+(i*16),font1);
            Brain::renderText(renderer, std::to_string(ver[i]->getId()).c_str(), 780,400+(i*16),font1);
        }*/

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
