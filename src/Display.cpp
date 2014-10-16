using namespace std;
#include <iostream>
#include "Display.h"
#include "Simulation.h"

Display::Display(void *sim){
    this->sim = sim;
    camera_x = SCREEN_WIDTH/2;
    camera_y = SCREEN_HEIGHT/2;
}
Display::~Display() {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::initDisplay() {
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "SDL Init failed" << endl;
        return;
    }

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "SwarmSim",                       // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        SCREEN_WIDTH,                      // width, in pixels
        SCREEN_HEIGHT,                     // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    if (window == NULL) {
        cout << "Could not create window" << endl;
        return;
    }

    ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (ren == NULL) {
        cout << "Could not create renderer" << endl;
        return;
    }
}

void Display::drawDisplay() {

    //Set background color
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
    SDL_RenderClear(ren);

    //Check the event queue
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym){
                case SDLK_q:
                case SDLK_ESCAPE:
                    exit(0);
                    break;
                default:
                    break;
            }
        }
    }

    Point2d center;
    ((Simulation*)sim)->getCenterOfMass(&center);
    //cout << "Center " << center.x << " " << center.y << endl;

    SDL_Rect rect = {static_cast<int>(camera_x-center.x),
                static_cast<int>(camera_y-center.y),5,5}; 

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderFillRect(ren, &rect);

    std::vector<Agent*>* agents = ((Simulation*)sim)->getAgents();

    double minX = (*agents)[0]->getLocationX();
    double minY = (*agents)[0]->getLocationY();
    double maxX = minX;
    double maxY = minY;

    SDL_SetRenderDrawColor(ren, 0, 180, 0, 0);

    std::vector<Vector2d*> queued_velocities;
    
    for(unsigned int i=0; i<agents->size(); i++) {
        rect.x = (*agents)[i]->getLocationX()+camera_x-center.x;
        rect.y = (*agents)[i]->getLocationY()+camera_y-center.y;
        if((*agents)[i]->getLocationX() < minX) minX = (*agents)[i]->getLocationX();
        if((*agents)[i]->getLocationY() < minY) minY = (*agents)[i]->getLocationY();
        if((*agents)[i]->getLocationX() > maxX) maxX = (*agents)[i]->getLocationX();
        if((*agents)[i]->getLocationY() > maxX) maxY = (*agents)[i]->getLocationY();

        SDL_RenderFillRect(ren, &rect);

        Vector2d v = (*agents)[i]->getVelocity();
        SDL_RenderDrawLine(ren,
                                 rect.x+2,
                                 rect.y+2,
                                 rect.x+2+v.getX()*2,
                                 rect.y+2+v.getY()*2);
    }

    SDL_SetRenderDrawColor(ren, 200, 200, 200, 0);
    minX = minX-fmod(minX, 50.)-400.;
    minY = minY-fmod(minY, 50.)-400.;
    for(int x = minX; x <maxX+400; x+=50.) {
        for(int y = minY; y < maxY+400; y+=50.) {
            rect.x = x+camera_x-center.x;
            rect.y = y+camera_y-center.y;
            SDL_RenderFillRect(ren, &rect);
        }
    }

    SDL_RenderPresent(ren);

    SDL_Delay(this->FRAME_DELAY);
}
