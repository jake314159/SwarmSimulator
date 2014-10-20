#ifndef DISPLAY_INCLUDE_GUARD_H
#define DISPLAY_INCLUDE_GUARD_H 1

#include <SDL.h>
#include <SDL_ttf.h>

class Display {

    private:
        SDL_Window *window;
        SDL_Renderer* ren;
        TTF_Font *font;
        void *sim;
        const static int SCREEN_WIDTH = 800;
        const static int SCREEN_HEIGHT = 580;
        const static int fontSize = 18;
        const static int FRAME_DELAY = 30;
        char* fontFile;

        double camera_x;
        double camera_y;

        void draw_frame_number();
        char score_buffer[30];
    public:

        Display(void *sim);
        ~Display();
        void initDisplay();
        void drawDisplay();

};

#endif

