
#include "Display.h"
#include "Simulation.h"

using namespace std;

#ifndef ENVIRONMENT_INCLUDE_GUARD_H
#define ENVIRONMENT_INCLUDE_GUARD_H 1

class Environment {
    public:

        Environment();

        int id;

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

        /* 
         * Runs at the start of every round. Should be used
         * to init everything needed for the environment
         * The argument is of type Simulation*
         */
        void (*roundStart)(void *);

        /*
         * Runs at the end of every round. Used to clean
         * up any memory used during the round. Bear in 
         * mind that destroy is also availble as a method
         * of clearing memory
         * The pointer is of type Simulation*
         */
        void (*roundEnd)(void *);

        /*
         * Runs once at the start of the simulation and
         * and is used to set-up anything that is needed
         * as part of the environment
         */
        void (*init)();

        /*
         * Runs at the end of the simulation and is used to
         * to clear up any memory allocations left over
         *
         * IMPORTANT: This is NOT guarentted to be run and
         * often wont be if the simulation has been killed
         * by the user
         */
        void (*destroy)();
};

#endif

