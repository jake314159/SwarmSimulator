#ifndef DISPLAY_INCLUDE_GUARD_H
#define DISPLAY_INCLUDE_GUARD_H 1

#include <SDL.h>

class Display {

    private:
        SDL_Window *window;
        SDL_Renderer* ren;
        void *sim;
        const static int SCREEN_WIDTH = 800;
        const static int SCREEN_HEIGHT = 580;
        //const int fontSize = 48;
        const static int FRAME_DELAY = 30;
        //const char* fontFile = "fonts/sample.ttf";

        double camera_x;
        double camera_y;
    public:

        Display(void *sim);
        ~Display();
        void initDisplay();
        void drawDisplay();

};

#endif

