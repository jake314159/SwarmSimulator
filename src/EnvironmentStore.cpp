using namespace std;
#include <stdlib.h>
#include <iostream>
#include <math.h>
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
/////////// VORTEX ENVIRONMENT ///////////
//////////////////////////////////////////

// From https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
double pDistance(double x, double y, double x1, double y1, double x2, double y2) {

  double A = x - x1;
  double B = y - y1;
  double C = x2 - x1;
  double D = y2 - y1;

  double dot = A * C + B * D;
  double len_sq = C * C + D * D;
  double param = -1;
  if (len_sq != 0) //in case of 0 length line
      param = dot / len_sq;

  double xx, yy;

  if (param < 0) {
    xx = x1;
    yy = y1;
  }
  else if (param > 1) {
    xx = x2;
    yy = y2;
  }
  else {
    xx = x1 + param * C;
    yy = y1 + param * D;
  }

  double dx = x - xx;
  double dy = y - yy;
  return sqrt(dx * dx + dy * dy);
}

// x = the value to calulate the prob of
// when the mean = mu
// and the standard deviation = sd
double gaussien_PDF(double x, double mu, double sd) {
    double pi2_sqrt = sqrt(2*M_PI);
    double x_min_mu = x-mu;
    return (1/(sd*pi2_sqrt)) * exp( (-(x_min_mu*x_min_mu)) / (2*sd*sd));
}

double get_benefit(double Dis) {
    Dis /= 20;
    double mu, sd, scale;
    if(Dis>0) {
        // Infront
        mu = 0.48;
        sd = 0.3;
        scale = 66*0.3;
    } else {
        // Behind
        mu = -0.75;
        sd = 0.4;
        scale = 45*0.7;
    }
    return gaussien_PDF(Dis, mu, sd) * scale;
}

void environment_votex_onFrame(void *simulation) {
    Simulation *s = (Simulation*)simulation;
    Agent* agents = s->getAgents();
    int flockSize = s->flockSize;

    for(unsigned int i=0; i<flockSize; i++) {
        // Find the nearest neighbour to i
        Agent *nn; 
        int nn_index = -1;
        double best_distance = -1;

        Point2d p; //Point where agent i is
        p.x = agents[i].getLocationX();
        p.y = agents[i].getLocationY();
        for(unsigned int j=0; j<flockSize; j++) {
            if(i==j) continue;

            double d = agents[j].distanceFrom(&p);
            if(nn_index<0 || d < best_distance) {
                best_distance = d;
                nn_index = j;
            }
        }

        nn = &agents[nn_index];

        //Get angle between velocities
        Vector2d v1 = agents[i].getVelocity();
        Vector2d v2 = nn->getVelocity();
        double angle = fabs(atan2(v2.getY(), v2.getX()) - atan2(v1.getY(), v1.getX()));

        //Find percentage of the velocity that is in the same direction
        double angle_percent = cos(angle);

        // Calculate distance from line Dis

        double Dis = pDistance( agents[i].getLocationX(), agents[i].getLocationY(),
                    nn->getLocationX(), nn->getLocationY(),
                    //Note these are permendicular
                    (nn->getLocationX()+v1.getY())*-1, nn->getLocationY()+v1.getX()
         );

        //Calculate how our position benefits us (as a maximum)
        double benefit = get_benefit(Dis);
        //cout << "Dis = " << Dis << "\t\tBenefit = "<< benefit << endl;

        //Multiply by 10 as it will be converted to an int so will lose anything after the dp.
        cout << 10 * benefit*angle_percent << endl;
        agents[i].score += 10 * benefit*angle_percent;
    }
}

//////////////////////////////////////////
/////////// SPREAD ENVIRONMENT ///////////
//////////////////////////////////////////
bool ENVIRONMENT_SPREAD_MIN = true;
void environment_spread_setMinimise(bool mini) {
    ENVIRONMENT_SPREAD_MIN = mini;
}

void environment_spread_onFrame(void *simulation) {
    Simulation *s = (Simulation*)simulation;
    Agent* agents = s->getAgents();
    int flockSize = s->flockSize;
    Point2d center;
    s->getCenterOfMass(&center);

    for(unsigned int i=0; i<flockSize; i++) {
        double xd = center.x - agents[i].getLocationX();
        double yd = center.y - agents[i].getLocationY();
        double distance_sq = xd*xd + yd*yd;
        if(ENVIRONMENT_SPREAD_MIN) distance_sq *= -1;
        agents[i].score += distance_sq;
    }
}

//////////////////////////////////////////
///////////// NND ENVIRONMENT ////////////
//////////////////////////////////////////
bool ENVIRONMENT_NND_MIN = true;
void environment_nnd_setMinimise(bool mini) {
    ENVIRONMENT_NND_MIN = mini;
}

void environment_nnd_onFrame(void *simulation) {
    Simulation *s = (Simulation*)simulation;
    Agent* agents = s->getAgents();
    int flockSize = s->flockSize;
    Point2d center;
    s->getCenterOfMass(&center);

    for(unsigned int i=0; i<flockSize; i++) {
        // Find the nearest neighbour to i
        Agent *nn; 
        int nn_index = -1;
        double best_distance = -1;

        Point2d p; //Point where agent i is
        p.x = agents[i].getLocationX();
        p.y = agents[i].getLocationY();
        for(unsigned int j=0; j<flockSize; j++) {
            if(i==j) continue;

            double d = agents[j].distanceFrom(&p);
            if(nn_index<0 || d < best_distance) {
                best_distance = d;
                nn_index = j;
            }
        }

        nn = &agents[nn_index];


        if(ENVIRONMENT_NND_MIN) best_distance *= -1;
        agents[i].score += best_distance;
    }
}

//////////////////////////////////////////
/////////// POLARITY ENVIRONMENT /////////
//////////////////////////////////////////
bool ENVIRONMENT_POL_MIN = true;
void environment_polarity_setMinimise(bool mini) {
    ENVIRONMENT_POL_MIN = mini;
}

void environment_polarity_onFrame(void *simulation) {
    Simulation *s = (Simulation*)simulation;
    Agent* agents = s->getAgents();
    int flockSize = s->flockSize;
    Point2d center;
    s->getCenterOfMass(&center);

    Vector2d aV; //Average velocity

    for(unsigned int i=0; i<flockSize; i++) {
        Vector2d v = agents[i].getVelocity();
        aV += v;

        //if(ENVIRONMENT_NND_MIN) best_distance *= -1;
        //agents[i].score += best_distance;
    }

    aV /= flockSize;
    aV /= aV.getMagnitude();

    for(unsigned int i=0; i<flockSize; i++) {
        Vector2d v = agents[i].getVelocity();
        v /= v.getMagnitude();
        double dot = aV.getX()*v.getX() + aV.getY()*v.getY();
        double pol = acos(dot);
        double HALF_PI = M_PI/2;
        double pol_star = (HALF_PI-pol)/HALF_PI;

        if(ENVIRONMENT_POL_MIN) pol_star *= -1;

        //MUST *1000 as it's being rounded to an int and the range is 0-1
        agents[i].score += pol_star*1000;
    }
}

//////////////////////////////////////////
//// CENTER DISPLACEMENT ENVIRONMENT /////
//////////////////////////////////////////

/* displacement min/max */
//Should we minimise center displacement (true) or maximise (false)??
bool ENVIRONMENT_CENTER_DISP_MIN = true;
void environment_center_disp_setMinimise(bool mini) {
    ENVIRONMENT_CENTER_DISP_MIN = mini;
}

Point2d last_center;
void environment_center_disp_onFrame(void *simulation) {
    Simulation *s = (Simulation*)simulation;
    Agent* agents = s->getAgents();
    int flockSize = s->flockSize;
    Point2d center;
    s->getCenterOfMass(&center);

    // Vector for the movement of the center of mass
    double vx = center.x - last_center.x;
    double vy = center.y - last_center.y;

    for(unsigned int i=0; i<flockSize; i++) {
        Vector2d a = agents[i].getVelocity();
        // A small angle means the agent helped cause the center of mass to move
        // * 100 makes it a large number as it will be converted to an int soon
        double angle = fabs(atan2(a.getY(),a.getX()) - atan2(vy,vx));
        angle *= 100;
        //cout << angle << endl;
        if(!ENVIRONMENT_CENTER_DISP_MIN) angle *= -1;
        agents[i].score += angle;
    }

    last_center = center;
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
