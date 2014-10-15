using namespace std;
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "Simulation.h"

Simulation::Simulation(int flockSize) {
    for(int i=flockSize; i>0; i--) {
        this->agents.push_back(new Agent());
    }
    cout << "Created a flock of " << this->agents.size() << endl;
}

//TODO test
void Simulation::getKNN(Point2d *p, Agent *knn[], int number) {
    std::vector<double> knn_distance;

    double largest_distance = -1.0;
    for(int k=0; k<number; k++) {
        knn[k] = this->agents[k];
        knn_distance[k] = knn[k]->distanceFrom(p);
        if(largest_distance <0 || knn_distance[k] > largest_distance) {
            largest_distance = knn_distance[k];
        }
    }
    
    for(unsigned int i=number; i<this->agents.size(); i++) {
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
        cout << "Frame " << this->runTime << endl;
        for(unsigned int i=0; i<this->agents.size(); i++) {
            //Update agents[i]
        }

        this->runTime++;
    }
}

void Simulation::reset() {
    srand(time(NULL));
    double spread = 100.0;
    Vector2d *v = new Vector2d();
    for(unsigned int i=0; i<this->agents.size(); i++) {
        this->agents[0]->setLocation(
                ((double)rand()/(double)RAND_MAX)*spread, 
                ((double)rand()/(double)RAND_MAX)*spread
            );
        double d = ((double)rand()/(double)RAND_MAX)*2. -1.;

        //Set Y so speed = 1
        double y_val = sqrt(1.0 - d*d);
        if( (rand()&1) == 0) {
            y_val = -y_val;
        }    
        v->setX(d);
        v->setY(y_val); 
        cout << "(" << d << "," << y_val << ")" << endl;
    }
    delete v;
}
