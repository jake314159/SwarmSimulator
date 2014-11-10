#include "Display.h"
#include "Simulation.h"

#ifndef ENVIRONMENT_INCLUDE_GUARD_H
#define ENVIRONMENT_INCLUDE_GUARD_H 1

class Environment {
    public:

        /*
         * Runs after a frame has been drawn but hasn't been
         * presented. Allows the environment to add something
         * to the screen. Note: Not all frames are drawn so
         * this should ONLY be to give some feedback on screen
         */ 
        void (*onDraw)(Display *);

        /*
         * Runs after a frame has been calculated
         * The pointer supplied is for a Simulation object
         * but due to cyclic includes it can't be explicit
         * //TODO Explicitly specify as a Simulation pointer
         */
        void (*onFrame)(void *);
};

#endif

