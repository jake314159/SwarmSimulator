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

/* Food */
/*
typedef struct {
    double x, y;
} FOOD;

int foodCount;
double spread = 400;
FOOD *food;
void environment_food_init(int numberOfFood) {
    int i;
    foodCount = numberOfFood;
    food = (FOOD*)malloc(sizeof(FOOD)*foodCount);
    for(i=0; i<foodCount; i++) {
        food[i].x = ((double)rand()/(double)RAND_MAX)*spread*2 - spread;
        food[i].y = ((double)rand()/(double)RAND_MAX)*spread*2 - spread;
    }
}

void environment_food_destroy() {
    free(food);
}

void environment_food_onDraw(Display* d) {
    int i;
    for(i=0; i<foodCount; i++) {
        d->drawBox(food[i].x, food[i].y, 3, 255, 0, 0);
    }
}

void environment_food_onFrame(void *simulation) {
    Simulation *s = (Simulation*)simulation;
    Agent* agents = s->getAgents();
    Point2d p;
    s->getCenterOfMass(&p);
    unsigned int i,j;
    for(j=0; j<foodCount; j++) {
        double minDistance = -1;
        for(i=0; i<s->flockSize; i++) {
            double d = ((agents[i].getLocationX()-food[j].x)*(agents[i].getLocationX()-food[j].x)+
                (agents[i].getLocationY()-food[j].y)*(agents[i].getLocationY()-food[j].y));

            if(minDistance <0 || minDistance > d) {
                minDistance = d;
            }
            if( d<3 ) {
                s->incScore(1);
                food[j].x = ((double)rand()/(double)RAND_MAX)*spread - spread/2;
                food[j].y = ((double)rand()/(double)RAND_MAX)*spread - spread/2;
            }
        }

        //We've moved away from the food so lets move it closer
        if(minDistance > spread*spread) {
            food[j].x = ((double)rand()/(double)RAND_MAX)*spread*2 - spread + p.x;
            food[j].y = ((double)rand()/(double)RAND_MAX)*spread*2 - spread + p.y;
        }
    }
}
*/
