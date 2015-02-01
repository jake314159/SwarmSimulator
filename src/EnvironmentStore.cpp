using namespace std;
#include <stdlib.h>
#include <iostream>
#include "Display.h"
#include "Simulation.h"

#define SCAN_N 8
#define SCAN_PENALTY_POW 4

struct S_IND
{
    int index;
    double x;
};


void scan_array_fill(Agent* agents, unsigned int flockSize, vector<struct S_IND> &l, double (*getValue)(Agent*)) {

    for(unsigned int i=0; i<flockSize; i++) {
        if(l.size()<SCAN_N) {
            bool success = false;
            for(unsigned int j=0; j<l.size(); j++) {
                if(getValue(&agents[i])>l[j].x) {
                    struct S_IND ind;
                    ind.x = getValue(&agents[i]);
                    ind.index = i;
                    l.insert(l.begin() + j, ind);
                    break;
                }
            }
            if(!success) {
                struct S_IND ind;
                ind.x = getValue(&agents[i]);
                ind.index = i;
                l.push_back(ind);
            }
        } else {
            for(unsigned int j=0; j<l.size(); j++) {
                if(getValue(&agents[i])>l[j].x) {
                    struct S_IND ind;
                    ind.x = getValue(&agents[i]);
                    ind.index = i;
                    l.insert(l.begin() + j, ind);
                    l.pop_back();
                    break;
                }
            }
        }
    }
}

void environment_scan_init(){}
void environment_scan_destroy(){}
void environment_scan_onDraw(Display* d){}

double agent_X_left(Agent *a) {return a->getLocationX();}
double agent_X_right(Agent *a) {return -a->getLocationX();}
double agent_Y_left(Agent *a) {return a->getLocationY();}
double agent_Y_right(Agent *a) {return -a->getLocationY();}
void environment_scan_onFrame(void *simulation) {
    Simulation *s = (Simulation*)simulation;
    Agent* agents = s->getAgents();
    long frame = s->getRunTime();

    // X LEFT
    vector<struct S_IND> l;
    scan_array_fill(agents, s->flockSize, l, 
        &agent_X_left
        );

    int delta = pow(2,SCAN_N);
    for(unsigned int j=0; j<l.size(); j++) {
        agents[l[j].index].score -= delta;
        delta /= 2;
    }

    //X RIGHT
    l.empty();
        scan_array_fill(agents, s->flockSize, l, 
        &agent_X_right
        );


    delta = pow(SCAN_PENALTY_POW,SCAN_N);
    for(unsigned int j=0; j<l.size(); j++) {
        agents[l[j].index].score -= delta;
        delta /= SCAN_PENALTY_POW;
    }

    // Y LEFT
    l.empty();
    scan_array_fill(agents, s->flockSize, l, 
        &agent_Y_left
        );

    delta = pow(2,SCAN_N);
    for(unsigned int j=0; j<l.size(); j++) {
        agents[l[j].index].score -= delta;
        delta /= 2;
    }

    //Y RIGHT
    l.empty();
        scan_array_fill(agents, s->flockSize, l, 
        &agent_Y_right
        );


    delta = pow(SCAN_PENALTY_POW,SCAN_N);
    for(unsigned int j=0; j<l.size(); j++) {
        agents[l[j].index].score -= delta;
        delta /= SCAN_PENALTY_POW;
    }

}


//////////////////////////////////////////
//////////// FOOD ENVIRONMENT ////////////
//////////////////////////////////////////

#define GRID_RESOLUTION 10.0
#define GRID_SIZE 1000
bool food_grid[GRID_SIZE][GRID_SIZE] = {{false}};

void environment_food_init() {

}

void environment_food_destroy() {

}

void environment_food_round_start(void *simulation) {
    for(int x=0; x<GRID_SIZE; x++) {
        for(int y=0; y<GRID_SIZE; y++) {
            food_grid[x][y] = false;
        }
    }
}

int convert_to_grid_index(double d) {
    return (int)( (d+(GRID_RESOLUTION*(GRID_SIZE/2))) / (GRID_RESOLUTION) );
}

void environment_food_onFrame(void *simulation) {
    Simulation *s = (Simulation*)simulation;
    Agent* agents = s->getAgents();
    int flockSize = s->flockSize;
    for(unsigned int i=0; i<flockSize; i++) {
        int x = convert_to_grid_index(agents[i].getLocationX());
        int y = convert_to_grid_index(agents[i].getLocationY());

        //Check they are on the grid first
        if(x<0 || x>GRID_SIZE || y<0 || y>GRID_SIZE) continue;

        if( !food_grid[x][y] ) {
            agents[i].score += 1;
            food_grid[x][y] = true;
        }
    }
}

