#ifndef SIMULATION_INCLUDE_GUARD_H
#define SIMULATION_INCLUDE_GUARD_H 1
using namespace std;
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
        void getKNN(Point2d *p, Agent *knn[], int number, unsigned int ignore_index);
        Vector2d getProjectionVector(unsigned int i, std::vector<char> &bin);
        double getApearSize(unsigned int i, unsigned int j);

        // Agent size and shape info
        static const double raw_size = 3.;
        static const double shape_ratio = 2;
        static const double shape_dif = 0;//M_PI/2;
        static const double speed = 2.0;

        //How often to update the agents velocities (in frames)
        static const int update_rate = 10;
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
        void enableRecord(std::string location);
};


#endif
