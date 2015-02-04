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

//////////////////////////////////////////
///////// INTERSECT ENVIRONMENT //////////
//////////////////////////////////////////

double calc_intersection(double x1, double y1, double x2, double y2, double r) {
    double r2 = r*r;
    double distance = sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
    double theta = 2 * acos( (0.5*distance)/(r) );
    return r2 * (theta - sin(theta));
}

void environment_intersect_onFrame(void *simulation) {
    Simulation *s = (Simulation*)simulation;
    Agent* agents = s->getAgents();
    int flockSize = s->flockSize;

    //Raius (How close you need to be to see an agent)
    double r = 100;

    for(unsigned int i=0; i<flockSize; i++) {
        for(unsigned int j=0; j<flockSize; j++) {
            if(i != j) {
                double intersect = calc_intersection(agents[i].getLocationX(), agents[i].getLocationY(),
                                        agents[j].getLocationX(), agents[j].getLocationY(), r);
                if(intersect > 0) {
                    agents[i].score += intersect;
                }
            }
        }
    }
}


//////////////////////////////////////////
//////////// MEASURE DESCRIBE ////////////
//////////////////////////////////////////

double MEASURE_DECR_SUM_SPREAD = 0.0;
double MEASURE_DECR_SUM_SPEED = 0.0;
int MEASURE_DECR_COUNT = 0;

Point2d MEASURE_DECR_CENTER_LAST; //x and y

void measure_describe_init() {
    MEASURE_DECR_SUM_SPREAD = 0.0;
    MEASURE_DECR_SUM_SPEED = 0.0;
    MEASURE_DECR_COUNT = 0;
}

void measure_describe_round_start(void *simulation) {
    Simulation *s = (Simulation*)simulation;
    s->getCenterOfMass(&MEASURE_DECR_CENTER_LAST);
}

void measure_describe_round_end(void *simulation) {
    Simulation *s = (Simulation*)simulation;
    Agent* agents = s->getAgents();
    long frame = s->getRunTime();

    if(frame < 1000 || frame > 3000) return;

    Point2d new_center;
    s->getCenterOfMass(&new_center);


    //Spread
    double spread_temp = 0.0;
    int flockSize = s->flockSize;
    for(unsigned int i=0; i<flockSize; i++) {
        double x = (agents[i].getLocationX() - new_center.x);
        double y = (agents[i].getLocationY() - new_center.y);

        spread_temp += sqrt( x*x + y*y );
    }
    spread_temp /= (double)flockSize;
    MEASURE_DECR_SUM_SPREAD += spread_temp;

    //Speed

    double a = new_center.x - MEASURE_DECR_CENTER_LAST.x;
    double b = new_center.y - MEASURE_DECR_CENTER_LAST.y;
    double position_change = sqrt( a*a + b*b )/((double)flockSize);

    MEASURE_DECR_SUM_SPEED += position_change;

    MEASURE_DECR_COUNT++;
}

void measure_describe_get_vals(double *spread, double *speed) {
    *spread = MEASURE_DECR_SUM_SPREAD/((double)MEASURE_DECR_COUNT);
    *speed = MEASURE_DECR_SUM_SPEED/((double)MEASURE_DECR_COUNT);
}

void measure_describe_destroy() {
    double a,b;
    measure_describe_get_vals(&a, &b);
    double speed = 4.3; //We can't access this so it's hard coded :'(
    cout << "Measure describe = (" << a << "," << (b/speed)*100 << ")" << endl;
}
