#ifndef SIMULATION_INCLUDE_GUARD_H
#define SIMULATION_INCLUDE_GUARD_H 1

#include <vector>
#include "SwarmValues.h"
#include "Agent.h"
#include "Display.h"

class Simulation {
    private:
        std::vector<Agent*> agents;
        SwarmValues *values;
        Display *display;
        //Environment *env;
        int score;
        long runTime; //In steps
        long maxRunTime;
        void getKNN(Point2d *p, Agent *knn[], int number, int ignore_index);
    public:
        Simulation(int flockSize, SwarmValues *values);
        void runSimulation(long maxRunTime);
        void setSwarmValues(SwarmValues *values);
        //void setEnvironment(Environment *env);
        void reset();
        void getCenterOfMass(Point2d *p);

        std::vector<Agent*>* getAgents();
        void addDisplay();
        long getRunTime();
};


#endif
