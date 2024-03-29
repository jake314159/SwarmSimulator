using namespace std;
#include <iostream>
#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
    #include <GL/glu.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "Display.h"
#include "Simulation.h"
#include "SDL_functions.h"

void Screenshot(int x, int y, int w, int h, const char * filename)
{
    unsigned char * pixels = new unsigned char[w*h*4]; // 4 bytes for RGBA
    glReadPixels(x,y,w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    SDL_Surface * surf = SDL_CreateRGBSurfaceFrom(pixels, w, h, 8*4, w*4, 0,0,0,0);
    IMG_SavePNG(surf, filename);

    SDL_FreeSurface(surf);
    delete [] pixels;
}

void Display::draw_frame_number()
{
    long timer = ((Simulation*)sim)->getRunTime();
    sprintf(score_buffer, "%ld", timer); //TODO The 8 is how often the simulation updates //TODO turn into not a magic number
    SDL_Color score_color = { 0, 0, 0 };
    SDL_Texture *font_image = renderText(score_buffer, font, score_color, ren);
    if (font_image == NULL){
        exit(1);
    }            
    renderTexture(font_image, ren, 5, 5);
}

void Display::draw_string(std::string text, int x, int y)
{
    SDL_Color score_color = { 0, 0, 0 };
    SDL_Texture *font_image = renderText((char*)text.c_str(), font, score_color, ren);
    if (font_image == NULL){
        exit(1);
    }            
    renderTexture(font_image, ren, x, y);
}

void Display::draw_int_number(int i, int x, int y)
{
    sprintf(score_buffer, "%d", i);
    SDL_Color score_color = { 0, 0, 0 };
    SDL_Texture *font_image = renderText(score_buffer, font, score_color, ren);
    if (font_image == NULL){
        exit(1);
    }            
    renderTexture(font_image, ren, x, y);
}

Display::Display(void *sim){
    this->sim = sim;
    camera_x = SCREEN_WIDTH/2;
    camera_y = SCREEN_HEIGHT/2;
    speed = 0;
    print = false;
    enable_record = false;
    onDraw = 0;
    zoom = 1.0;
}
Display::~Display() {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::setup_record(std::string location) {
        enable_record = true;
        save_location = location;
        print = false;
        time_of_record = 0;
}

void Display::setOnDrawFunction(void (*onDraw)(Display *)) {
    this->onDraw = onDraw;
}

void Display::initDisplay() {
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "SDL Init failed" << endl;
        return;
    }

    if (TTF_Init() != 0) {
      cerr << "TTF_Init() Failed: " << TTF_GetError() << endl;
      SDL_Quit();
      exit(1);
    }

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "SwarmSim",                       // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        SCREEN_WIDTH,                      // width, in pixels
        SCREEN_HEIGHT,                     // height, in pixels
        0                                  // flags
    );

    if (window == NULL) {
        cout << "Could not create window" << endl;
        return;
    }

    ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    if (ren == NULL) {
        cout << "Could not create renderer" << endl;
        return;
    }

    font = TTF_OpenFont("fonts/sample.ttf", fontSize);
	if (font == NULL){
		cout << "TTF_OpenFont" << endl;
	}

    //Render an empty frame while waiting to show the images
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);
}

void Display::drawDisplay() {
    
    //Check the event queue
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym){
                case SDLK_q:
                case SDLK_ESCAPE:
                    exit(0);
                    break;
                case SDLK_SPACE:
                    if(speed == 0)  speed = 1;
                    else            speed = 0;
                    break;
                case SDLK_z:
                    if(speed>1) speed--;
                    break;
                case SDLK_x:
                    speed++;
                    break;
                case SDLK_c:
                    zoom -= 0.1;
                    break;
                case SDLK_v:
                    zoom += 0.1;
                    break;
                case SDLK_r:
                    char buf[100];
                    if(enable_record && time_of_record == 0) {
                        time_of_record = time(NULL);
                        sprintf(buf, "%s/%ld", save_location.c_str(), time_of_record);
                        mkdir(buf, 0700);
                    }
                    print = true;
                    break;
                default:
                    break;
            }
        } else if(e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym){
                case SDLK_r:
                    print = false;
                    time_of_record = 0;
                    break;
                default:
                    break;
            }
        }
    }

    //we are paused
    if(speed == 0) {
        bool paused = true;
        while(paused) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym){
                        case SDLK_q:
                        case SDLK_ESCAPE:
                            exit(0);
                            break;
                        case SDLK_SPACE:
                            paused = false;
                            speed = 1;
                            break;
                        case SDLK_s:
                            //show 1 frame step
                            paused = false;
                            //ensure we aren't on a skipped frame
                            count = speed+1;
                            break;
                    }
                }
            }
            SDL_Delay(this->FRAME_DELAY);
        }
        
    }

    if(count>speed) {
        count = 0;
    } else {
        count++;
        return;
    }

    //Set background color
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
    SDL_RenderClear(ren);

    ((Simulation*)sim)->getCenterOfMass(&center);

    SDL_Rect rect = {static_cast<int>(camera_x-center.x),
                static_cast<int>(camera_y-center.y),3,3}; 

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderFillRect(ren, &rect);

    Agent* agents = ((Simulation*)sim)->getAgents();
    int flockSize = ((Simulation*)sim)->flockSize;

    double minX = (agents)[0].getLocationX();
    double minY = (agents)[0].getLocationY();
    double maxX = minX;
    double maxY = minY;

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);

    double agent_length = ((Simulation*)sim)->raw_size;
    double agent_width = agent_length / ((Simulation*)sim)->shape_ratio;
    for(unsigned int i=(flockSize-1); i>0; i--) {
        rect.x = (agents)[i].getLocationX()+camera_x/zoom-center.x;
        rect.y = (agents)[i].getLocationY()+camera_y/zoom-center.y;
        if((agents)[i].getLocationX() < minX) minX = (agents)[i].getLocationX();
        if((agents)[i].getLocationY() < minY) minY = (agents)[i].getLocationY();
        if((agents)[i].getLocationX() > maxX) maxX = (agents)[i].getLocationX();
        if((agents)[i].getLocationY() > maxY) maxY = (agents)[i].getLocationY();

        //if(i==0) SDL_SetRenderDrawColor(ren, 0, 180, 0, 0);
        //SDL_RenderFillRect(ren, &rect);
        //if(i==0) SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);

        Vector2d v = (agents)[i].getVelocity();
        v /= v.getMagnitude();
        v *= agent_length;

        //float angle = atan2f(-v.getY(), v.getX());

        Vector2d perp_v;

        perp_v.setX( -v.getY() );//cos(angle)*agent_width );
        perp_v.setY( v.getX() ); //sin(angle)*agent_width );
        perp_v /= perp_v.getMagnitude();
        perp_v *= agent_width;

        SDL_RenderDrawLine(ren,
                         (rect.x+1)*zoom,
                         (rect.y+1)*zoom,
                         (rect.x+1-v.getX())*zoom,
                         (rect.y+1-v.getY())*zoom
                            );
        SDL_RenderDrawLine(ren,
                         (rect.x+1-v.getX()-perp_v.getX())*zoom,
                         (rect.y+1-v.getY()-perp_v.getY())*zoom,
                         (rect.x+1-v.getX()+perp_v.getX())*zoom,
                         (rect.y+1-v.getY()+perp_v.getY())*zoom
                         );
        SDL_RenderDrawLine(ren,
                 (rect.x+1)*zoom,
                 (rect.y+1)*zoom,
                 (rect.x+1-v.getX()+perp_v.getX())*zoom,
                 (rect.y+1-v.getY()+perp_v.getY())*zoom
                 );
        SDL_RenderDrawLine(ren,
                 (rect.x+1)*zoom,
                 (rect.y+1)*zoom,
                 (rect.x+1-v.getX()-perp_v.getX())*zoom,
                 (rect.y+1-v.getY()-perp_v.getY())*zoom
                 );
    }

    SDL_SetRenderDrawColor(ren, 200, 200, 200, 0);
    minX = minX-fmod(minX, 100.)-400.;
    minY = minY-fmod(minY, 100.)-400.;
    rect.w = 5*zoom;
    rect.h = 5*zoom;
    for(int x = minX-800; x <maxX+800; x+=100.) {
        for(int y = minY-400; y < maxY+400; y+=100.) {
            rect.x = (x+camera_x-center.x)*zoom;
            rect.y = (y+camera_y-center.y)*zoom;
            SDL_RenderFillRect(ren, &rect);
        }
    }
    draw_frame_number();
    draw_int_number(((Simulation*)sim)->getScore(), 5, 30);
    draw_int_number(speed, 5, 60);
    draw_int_number(static_cast<int>(zoom*100), 5, 90);
    if(enable_record && print) {
        char buf[1000];
        sprintf(buf, "%s/%ld/%06ld.png", save_location.c_str(), time_of_record, ((Simulation*)sim)->getRunTime());
        Screenshot(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, buf);
        cout <<"PRINT!! '"<<buf<<"'"<<endl; 
    }
    if(onDraw != 0)
        onDraw(this);
    SDL_RenderPresent(ren);

    SDL_Delay(this->FRAME_DELAY);
}

void Display::drawBox(double x, double y, int size, char r, char g, char b) {
    SDL_SetRenderDrawColor(ren, r, g, b, 0);
    SDL_Rect rect = {static_cast<int>(x+camera_x-center.x),
                static_cast<int>(y+camera_y-center.y),size,size}; 
    SDL_RenderFillRect(ren, &rect);
}
