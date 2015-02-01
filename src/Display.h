#ifndef DISPLAY_INCLUDE_GUARD_H
#define DISPLAY_INCLUDE_GUARD_H 1

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Point2d.h"

/*
 * DISPLAY CONTROLS
 * 
 *   q : QUIT
 *   z : INCREASE SPEED
 *   x : DECREASE SPEED
 * 
*/

class Display {

    private:
        SDL_Window *window;
        SDL_Renderer* ren;
        TTF_Font *font;
        void *sim;
        const static int SCREEN_WIDTH = 1280;
        const static int SCREEN_HEIGHT = 720;
        const static int fontSize = 18;
        const static int FRAME_DELAY = 30;
        char* fontFile;

        double camera_x;
        double camera_y;
        Point2d center;

        int count;
        int speed;

        bool enable_record;
        std::string save_location;
        bool print;
        long time_of_record;

        void draw_frame_number();
        void draw_string(std::string text, int x, int y);
        void draw_int_number(int i, int x, int y);
        char score_buffer[30];

        void (*onDraw)(Display *);

        double zoom;
    public:

        Display(void *sim);
        ~Display();
        void initDisplay();
        void drawDisplay();
        void setup_record(std::string location);
        void setOnDrawFunction(void (*onDraw)(Display *));

        void drawBox(double x, double y, int size, char r, char g, char b);
};

#endif

