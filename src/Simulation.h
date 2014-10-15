#ifndef SIMULATION_INCLUDE_GUARD_H
#define SIMULATION_INCLUDE_GUARD_H 1

#include <vector>
#include "SwarmValues.h"
#include "Agent.h"

class Simulation {
    private:
        std::vector<Agent*> agents;
        SwarmValues *values;
        //Environment *env;
        int score;
        long runTime; //In steps
        long maxRunTime;
        void getKNN(Point2d *p, Agent *knn[], int number);
    public:
        Simulation(int flockSize);
        void runSimulation(long maxRunTime);
        void setSwarmValues(SwarmValues *values);
        //void setEnvironment(Environment *env);
        void reset();
        Point2d getCenterOfMass();
};


#endif
