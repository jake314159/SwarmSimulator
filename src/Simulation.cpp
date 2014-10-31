using namespace std;
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "Simulation.h"
#include "fastMath.h"

#define BIN_COUNT 100

Simulation::Simulation(int flockSize, SwarmValues *values) {
    this->flockSize = flockSize;
    agents = new Agent[flockSize];
    display = 0;
    this->values = *values;
    setup_fast_math();
    cout << "Created a flock of " << flockSize << endl;
}

Simulation::~Simulation() {
    delete agents;
    delete display;
}

long Simulation::getRunTime() {
    return this->runTime;
}

void Simulation::addDisplay()
{
    this->display = new Display(this);
    this->display->initDisplay();
}

void Simulation::enableRecord(std::string location) {
    if(display==NULL) cout << "ERROR: Display not initialised! Can't enable recording" << endl;
    display->setup_record(location);
}

//TODO test
//TODO what if store index values NOT pointers?
void Simulation::getKNN(const Point2d p, Agent *knn[], const int number, const unsigned int ignore_index) {
    std::vector<double> knn_distance;
    for(int i=0; i<number;i++) knn_distance.push_back(-1);

    double largest_distance = -1.0;
    int filled = 0;
    for(unsigned int k=(flockSize-1); filled<number; k--) {
        if(k == ignore_index) continue;

        knn[filled] = &(this->agents[filled]);
        knn_distance[filled] = knn[filled]->distanceFrom(&p);
        if(largest_distance <0 || knn_distance[filled] > largest_distance) {
            largest_distance = knn_distance[filled];
        }
        filled++;
    }
    
    for(unsigned int i=(flockSize-1); i>0; i--) {
        if(i == ignore_index) continue;

        double distance = agents[i].distanceFrom(&p);
        // If this should be put into our knn array
        if(distance < largest_distance) {
            //Find which one we should replace
            for(int k=0; k<number; k++) {
                if(knn_distance[k] == largest_distance) {
                    knn[k] = &(agents[i]);
                    knn_distance[k] = distance;
                    largest_distance = distance;
                    break;
                }
            }
        }        
    }
}

double Simulation::getApearSize(const unsigned int i, const unsigned int j) {
    Point2d l;
    l.x = agents[j].getLocationX();
    l.y = agents[j].getLocationY();

    Vector2d from;
    agents[i].vectorFrom(&l, &from);
    Vector2d other_velocity = agents[j].getVelocity();

    // Work out the number of bins used by this agent
    double dot_prod = other_velocity.getX()*(from.getX())+other_velocity.getY()*(from.getY());
    double cos_angle_working = dot_prod/(from.getMagnitude()*other_velocity.getMagnitude());
    double angle = fastacos(cos_angle_working);
    double sin_angle = fastsin(angle+shape_dif);
    double cos_angle = fastcos(angle+shape_dif);
    double apear_size = raw_size/(sqrt(sin_angle*sin_angle + (shape_ratio*shape_ratio*cos_angle*cos_angle)));
    return apear_size;
}

Vector2d Simulation::getProjectionVector(const unsigned int i, std::vector<char> &bin) {

    //Reset bin
    for(int j=0; j<BIN_COUNT;j++) bin[j] = false;

    Vector2d v_proj;
    v_proj.setVector(0.0,0.0);

    double binSize = (M_PI*2)/BIN_COUNT;

    Point2d l;
    for(unsigned int j=(flockSize-1); j>0; j--) {
        if( j == i) continue; //Skip self
        l.x = agents[j].getLocationX();
        l.y = agents[j].getLocationY();

        Vector2d from;
        agents[i].vectorFrom(&l, &from);

        double theta;// = acos(cos_theta);
        //
        //    2     |    1
        //          |
        //  --------+--------
        //          |
        //    3     |    4
        //
        
        if(from.getX() >=0 && from.getY()>=0) {
            //case 1
            theta = fastatan(from.getY()/from.getX());
        } else if(from.getX() <=0 && from.getY() >= 0) {
            //case 2
            theta = M_PI/2 + fastatan((-from.getX())/from.getY());
        } else if(from.getX() <=0 && from.getY() <=0) {
            //case 3
            theta = M_PI + fastatan((-from.getY())/(-from.getX()));
        } else { //from.getX()>=0 && y<=0
            //case4
            theta = M_PI + M_PI/2 + fastatan(from.getX()/(-from.getY()));
        }

        double apear_size = getApearSize(i,j);
        double size_angle = fastatan(apear_size/(from.getMagnitude()));
        // The number of bins the agent fills (half as based on radius)
        int size_bins = (int)(size_angle/binSize);

        int bin_N = (int)((theta)/binSize);
        for(int k=-size_bins; k<size_bins; k++) {
            if((bin_N+k)>=0) {
                bin[(bin_N+k)%BIN_COUNT] = true;
            } else {
                bin[(bin_N+k+BIN_COUNT)%BIN_COUNT] = true;
            }
        }
    }

    double boundry_count = 0.0;
    for(int j=0; j<(BIN_COUNT);j++) {
        if(bin[j] != bin[(j+1)%BIN_COUNT]) {
            double theta = (j*binSize);
            v_proj.setX(v_proj.getX() + fastcos(theta));
            v_proj.setY(v_proj.getY() + fastsin(theta));
            boundry_count += 1.0;
        }
    }

    if(v_proj.getMagnitude()>0)
        v_proj /= v_proj.getMagnitude();

    return v_proj;
}

void Simulation::runSimulation(const long maxRunTime) {
    this->maxRunTime = maxRunTime;
    this->runTime = 0;

    //Make the bins for finding the projection
    std::vector<char> bin;
    for(int j=0; j<BIN_COUNT;j++) bin.push_back(false);

    Agent **knn = new Agent*[4];

    while(this->maxRunTime >=0 && this->runTime < this->maxRunTime) {

        if( runTime%update_rate == 0) {
            for(unsigned int i=(flockSize-1); i>0; i--) {
                //Update agents[i]

                Point2d p;
                p.x = agents[i].getLocationX();
                p.y = agents[i].getLocationY();
                this->getKNN(p, knn, 4, i);

                Vector2d v_neig;
                v_neig.setVector(0.0,0.0);
                double mag = 0.0;
                for(int j=0; j<4; j++) {
                    Vector2d v2 = (knn[j]->getVelocity());
                    v_neig.setX(v_neig.getX()+v2.getX());
                    v_neig.setY(v_neig.getY()+v2.getY());
                    mag += v2.getMagnitude();
                }
                //Set magnitude to 1
                if(mag != 0.0) {
                    v_neig /= mag;
                }

                //Noise vector
                Vector2d v_rand;
                v_rand.randomize();

                // Find projection vector
                Vector2d v_proj = getProjectionVector(i, bin);

                //Set weights
                v_neig *= values.align_weight;
                v_rand *= values.noise_weight;
                v_proj *= values.proj_weight;

                // Sum all vectors into the v_neig
                v_neig.setX(v_neig.getX() + v_rand.getX());
                v_neig.setY(v_neig.getY() + v_rand.getY());

                v_neig.setX(v_neig.getX() + v_proj.getX());
                v_neig.setY(v_neig.getY() + v_proj.getY());

                //Fix speed
                if(v_neig.getMagnitude()>0)
                    v_neig /= v_neig.getMagnitude();
                v_neig *= speed;

                agents[i].updateVelocity(&v_neig);
                agents[i].updateLocation();
            }
        } else {
            // If a skip frame then don't change the velocity just move each
            // agent in the direction they were going
            for(unsigned int i=0; i<flockSize; i++) {
                agents[i].updateLocation();
            }
        }

        // If there is a display then draw it
        if(/*runTime > 1500 && */display != 0) {
            display->drawDisplay();
        }

        this->runTime++;
    }

    delete knn;
}

void Simulation::reset() {
    srand(time(NULL));
    double spread = 100.0;
    Vector2d *v = new Vector2d();
    for(unsigned int i=(flockSize-1); i>0; i--) {
        double x = ((double)rand()/(double)RAND_MAX)*spread;
        double y = ((double)rand()/(double)RAND_MAX)*spread;

        agents[i].setLocation(
                x, 
                y
            );
        double d = ((double)rand()/(double)RAND_MAX)*2. -1.;

        //Set Y so speed = 1
        double y_val = fastsqrt(1.0 - d*d);
        if( (rand()&1) == 0) {
            y_val = -y_val;
        }    
        v->setX(d);
        v->setY(y_val);
        agents[i].updateVelocity(v);
        agents[i].updateLocation();
    }
    delete v;
}

Agent* Simulation::getAgents() {
    return agents;
}

void Simulation::getCenterOfMass(Point2d *p) {
    p->x = 0;
    p->y = 0;

    for(unsigned int i=(flockSize-1); i>0; i--) {
        p->x += agents[i].getLocationX();
        p->y += agents[i].getLocationY();
    }

    p->x /= (double)flockSize;
    p->y /= (double)flockSize;
}

