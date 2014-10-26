using namespace std;
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "Simulation.h"

#define BIN_COUNT 100

Simulation::Simulation(int flockSize, SwarmValues *values) {
    for(int i=flockSize; i>0; i--) {
        this->agents.push_back(new Agent());
    }
    display = 0;
    this->values = values;
    cout << "Created a flock of " << this->agents.size() << endl;
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

        if( runTime%8 == 0) {
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
                double rand_val = ((double)rand()/(double)RAND_MAX)*2. -1.;

                //Set Y so speed = 1
                double y_val = sqrt(1.0 - rand_val*rand_val);
                if( (rand()&1) == 0) {
                    y_val = -y_val;
                }
                v_rand.setX(rand_val);
                v_rand.setY(y_val);
                //v_rand.setVector(((double)rand()/(double)RAND_MAX)*2-1.0,((double)rand()/(double)RAND_MAX)*2-1.0);

                //Make the bins
                double binSize = (M_PI*2)/BIN_COUNT;
                std::vector<char> bin;
                for(int j=0; j<BIN_COUNT;j++) bin.push_back(false);

                Vector2d v_proj;
                v_proj.setVector(0.0,0.0);

                Vector2d normal_v;
                normal_v.setVector(1.0, 0.0);

                Point2d l;
                for(unsigned int j=0; j<this->agents.size(); j++) {
                    if( j == i) continue; //Skip self
                    l.x = agents[j]->getLocationX();
                    l.y = agents[j]->getLocationY();
                    //cout << "agents["<<j<<"]=("<<l.x<<","<<l.y<<")"<<endl;
                    Vector2d* from = agents[i]->vectorFrom(&l);
                    //double dot_prod = normal_v.getX()*from->getX()+normal_v.getY()*from->getY();
                    //double cos_theta = dot_prod/(from->getMagnitude()*normal_v.getMagnitude());
                    double theta;// = acos(cos_theta);
                    //
                    //    2     |    1
                    //          |
                    //  --------+--------
                    //          |
                    //    3     |    4
                    //
                    
                    if(from->getX() >=0 && from->getY()>=0) {
                        //case 1
                        theta = atan(from->getY()/from->getX());
                    } else if(from->getX() <=0 && from->getY() >= 0) {
                        //case 2
                        theta = M_PI/2 + atan((-from->getX())/from->getY());
                    } else if(from->getX() <=0 && from->getY() <=0) {
                        //case 3
                        theta = M_PI + atan((-from->getY())/(-from->getX()));
                    } else { //from->getX()>=0 && y<=0
                        //case4
                        theta = M_PI + M_PI/2 + atan(from->getX()/(-from->getY()));
                    }

                    Vector2d other_velocity = agents[j]->getVelocity();
                    double raw_size = 3.;
                    double ratio = 2;
                    double dif = 0;//M_PI/2;
                    double dot_prod = other_velocity.getX()*(from->getX())+other_velocity.getY()*(from->getY());
                    double cos_angle_working = dot_prod/(from->getMagnitude()*other_velocity.getMagnitude());
                    double angle = acos(cos_angle_working);
                    double sin_angle = sin(angle+dif);
                    double cos_angle = cos(angle+dif);
                    double apear_size = raw_size/(sqrt(sin_angle*sin_angle + (ratio*ratio*cos_angle*cos_angle)));

                    //if(from->getY() > 0) theta += M_PI;
                    double size_angle = atan(apear_size/(agents[i]->distanceFrom(&l)));
                    int size_bins = (int)(size_angle/binSize);

                    //cout << size_bins << endl;
                    int bin_N = (int)((theta)/binSize);
                    //bin[bin_N] = true;
                    //cout <<theta<<" "<< size_angle << " " << size_bins << " " << agents[i]->distanceFrom(&l)<<endl;
                    for(int k=-size_bins; k<size_bins; k++) {
                        if((bin_N+k)>=0) {
                            bin[(bin_N+k)%BIN_COUNT] = true;
                        } else {
                            bin[(bin_N+k+BIN_COUNT)%BIN_COUNT] = true;
                        }
                    }
                }

                if(i==0) cout << this->runTime << "  ";
                Vector2d vt;
                double boundry_count = 0.0;
                for(int j=0; j<(BIN_COUNT);j++) {
                    if(i==0 && j%6==0)  {
                        cout <<(((int)bin[j]) ? "X" : "_");
                    }
                    if(bin[j] != bin[(j+1)%BIN_COUNT]) {
                        double theta = (j*binSize);
                        double mult = 1;
                        if(theta>M_PI) {
                            //theta = theta-M_PI;
                           // mult = -1;
                        }
                        //cout << theta << "   (" << cos(theta) <<","<<sin(theta)<<")"<<endl;
                        vt.setVector(cos(theta), sin(theta));
                        v_proj.setX(v_proj.getX() + vt.getX());
                        v_proj.setY(v_proj.getY() + vt.getY());
                        boundry_count += 1.0;
                    }
                }
                if(i==0) cout << endl;

                //cout << "v_proj==("<<v_proj.getX()<<","<<v_proj.getY()<<") / "<<boundry_count<<endl;
                if(v_proj.getMagnitude()>0)
                    v_proj /= v_proj.getMagnitude();

                v *= this->values->align_weight;
                v_rand *= this->values->noise_weight;
                v_proj *= this->values->proj_weight;

                v.setX(v.getX() + v_rand.getX());
                v.setY(v.getY() + v_rand.getY());

                v.setX(v.getX() + v_proj.getX());
                v.setY(v.getY() + v_proj.getY());

                if(v.getMagnitude()>0)
                    v /= v.getMagnitude();
                v *= 2; //the speed

                agents[i]->updateVelocity(&v);
                agents[i]->updateLocation();
            }
        } else {
            for(unsigned int i=0; i<this->agents.size(); i++) {
                agents[i]->updateLocation();
            }
        }

        // If there is a display then draw it
        if(/*runTime > 1500 && */display != 0) {
            display->drawDisplay();
        }

        this->runTime++;
    }
}

void Simulation::reset() {
    srand(time(NULL));
    double spread = 100.0;
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

