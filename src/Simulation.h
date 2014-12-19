#ifndef SIMULATION_INCLUDE_GUARD_H
#define SIMULATION_INCLUDE_GUARD_H 1
using namespace std;
#include <vector>
#include "SwarmValues.h"
#include "Agent.h"
#include "Display.h"
#include "Environment.h"

class Simulation {
    private:
        Agent* agents;
        SwarmValues values;
        Display *display;
        void (*onFrame)(void *);
        int score;
        long runTime; //In steps
        long maxRunTime;
        void getKNN(const Point2d p, Agent *knn[], const int number, const unsigned int ignore_index);
        Vector2d getProjectionVector(const unsigned int i, std::vector<char> &bin);
        double getApearSize(const unsigned int i, const unsigned int j);

        // Agent size and shape info

        static const double shape_dif = 0;//M_PI/2;
        static const double speed = 2.0;

        //How often to update the agents velocities (in frames)
        static const int update_rate = 7;//10;
    public:
        static const double raw_size = 5.;
        static const double shape_ratio = 7;

        unsigned int flockSize;

        Simulation(int flockSize, SwarmValues *values);
        ~Simulation();

        void runSimulation(const long maxRunTime);
        void setSwarmValues(SwarmValues *values);
        //void setEnvironment(Environment *env);
        void reset();
        void getCenterOfMass(Point2d *p);

        Agent* getAgents();
        void addDisplay();
        long getRunTime();
        void enableRecord(std::string location);

        void setEnvironment(Environment *env);
        void incScore(int v);
        int getScore();
        void combine_vectors(Vector2d &current, Vector2d &prefered);
};


#endif
