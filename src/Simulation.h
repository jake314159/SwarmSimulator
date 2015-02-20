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
        Environment *env;
        long long score;
        long runTime; //In steps
        long maxRunTime;
        void getKNN(const Point2d p, Agent *knn[], const int number, const unsigned int ignore_index);
        Vector2d getProjectionVector(const unsigned int i, std::vector<char> &bin);
        double getApearSize(const unsigned int i, const unsigned int j);

        // Agent size and shape info
         double shape_dif;//M_PI/2;
         double speed;

        //How often to update the agents velocities (in frames)
        static const int update_rate = 1;//10;

        int env_id;
        bool evolve; //Should we evolve better solutions?
        bool one_direction; //Should all agents start in one direction?
        bool hill_climb;

        int round_length;
    public:
        double raw_size;
        double shape_ratio;

        unsigned int flockSize;

        char* json_dir;

        Simulation(int flockSize, SwarmValues *values);
        ~Simulation();

        void runSimulation(const long maxRunTime);
        void setSwarmValues(SwarmValues *values);

        void reset();
        void getCenterOfMass(Point2d *p);

        Agent* getAgents();
        void addDisplay();
        long getRunTime();
        void enableRecord(std::string location);

        void setEnvironment(Environment *env);
        void incScore(int v);
        long long getScore();
        void setScore(long long score);
        void combine_vectors(Vector2d &current, Vector2d &prefered);

        void setEvolve(bool evolve);
        void setOneDirection(bool one_direction);
        void setHillClimb(bool hill_climb);

        void save_round(int round_number, Agent *pop, int flockSize);
        void load_json(string filename);
        int getRoundLength();
};


#endif
