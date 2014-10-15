using namespace std;
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "Simulation.h"

Simulation::Simulation(int flockSize, SwarmValues *values) {
    for(int i=flockSize; i>0; i--) {
        this->agents.push_back(new Agent());
    }
    display = 0;
    this->values = values;
    cout << "Created a flock of " << this->agents.size() << endl;
}

void Simulation::addDisplay()
{
    this->display = new Display(this);
    this->display->initDisplay();
}

//TODO test
void Simulation::getKNN(Point2d *p, Agent *knn[], int number, int ignore_index) {
    std::vector<double> knn_distance;
    for(int i=0; i<number;i++) knn_distance.push_back(-1);

    double largest_distance = -1.0;
    int filled = 0;
    for(int k=0; filled<number; k++) {
        if(k == ignore_index) continue;

        knn[filled] = this->agents[filled];
        knn_distance[filled] = knn[filled]->distanceFrom(p);
        if(largest_distance <0 || knn_distance[filled] > largest_distance) {
            largest_distance = knn_distance[filled];
        }
        filled++;
    }
    
    for(unsigned int i=number; i<this->agents.size(); i++) {
        if(i == ignore_index) continue;

        double distance = this->agents[i]->distanceFrom(p);
        // If this should be put into our knn array
        if(distance < largest_distance) {
            //Find which one we should replace
            for(int k=0; k<number; k++) {
                if(knn_distance[k] == largest_distance) {
                    knn[k] = this->agents[i];
                    knn_distance[k] = distance;
                    largest_distance = distance;
                    break;
                }
            }
        }        
    }
}

void Simulation::runSimulation(long maxRunTime) {
    this->maxRunTime = maxRunTime;
    this->runTime = 0;

    while(this->maxRunTime >=0 && this->runTime < this->maxRunTime) {

        for(unsigned int i=0; i<this->agents.size(); i++) {
            //Update agents[i]

            Point2d p;
            p.x = agents[i]->getLocationX();
            p.y = agents[i]->getLocationY();
            Agent **knn = new Agent*[4];
            this->getKNN(&p, knn, 4, i);

            Vector2d v;
            v.setVector(0.0,0.0);//((double)rand()/(double)RAND_MAX)*1-0.5,((double)rand()/(double)RAND_MAX)*1-0.5);
            double mag = 0.0;
            for(int j=0; j<4; j++) {
                Vector2d v2 = (knn[j]->getVelocity());
                v.setX(v.getX()+v2.getX());
                v.setY(v.getY()+v2.getY());
                mag += v2.getMagnitude();
            }

            if(mag != 0.0)
                v /= mag;

            Vector2d v_rand;
            v_rand.setVector(((double)rand()/(double)RAND_MAX)*2-1.0,((double)rand()/(double)RAND_MAX)*2-1.0);

            v *= this->values->align_weight;
            v_rand *= this->values->noise_weight;
            v.setX(v.getX() + v_rand.getX());
            v.setY(v.getY() + v_rand.getY());

            agents[i]->updateVelocity(&v);
            agents[i]->updateLocation();
        }

        // If there is a display then draw it
        if(display != 0 && runTime%3==0) {
            display->drawDisplay();
        }

        this->runTime++;
    }
}

void Simulation::reset() {
    srand(time(NULL));
    double spread = 200.0;
    Vector2d *v = new Vector2d();
    for(unsigned int i=0; i<this->agents.size(); i++) {
        double x = ((double)rand()/(double)RAND_MAX)*spread;
        double y = ((double)rand()/(double)RAND_MAX)*spread;

        this->agents[i]->setLocation(
                x, 
                y
            );
        double d = ((double)rand()/(double)RAND_MAX)*2. -1.;

        //Set Y so speed = 1
        double y_val = sqrt(1.0 - d*d);
        if( (rand()&1) == 0) {
            y_val = -y_val;
        }    
        v->setX(d);
        v->setY(y_val);
        this->agents[i]->updateVelocity(v);
        this->agents[i]->updateLocation();
    }
    delete v;
}

std::vector<Agent*>* Simulation::getAgents() {
    return &(this->agents);
}

void Simulation::getCenterOfMass(Point2d *p) {
    p->x = 0;
    p->y = 0;

    for(unsigned int i=0; i<this->agents.size(); i++) {
        p->x += this->agents[i]->getLocationX();
        p->y += this->agents[i]->getLocationY();
    }

    p->x /= agents.size();
    p->y /= agents.size();
}

