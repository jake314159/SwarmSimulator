using namespace std;
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <algorithm>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/writer.h"
#include "rapidjson/filestream.h"

#include "Simulation.h"
#include "fastMath.h"

#define BIN_COUNT 600


double rand_f_() {
    return static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
}

//TODO This doesn't need to be global.
double mutate_rate = 0.005;

double mutate_f(double f) {
    if((rand()&1)==0) {
        return f += rand_f_()*mutate_rate;
    } else {
        return f -= rand_f_()*mutate_rate;
    }
}

void Simulation::save_round(int round_number, Agent *pop, int flockSize) {
    char buff[256];

    sprintf(buff, "%s/%06d.json", json_dir, round_number);
    cout << "Write to '" << buff << "'" << endl;

    ofstream json;
    json.open (buff);

    if (!json.is_open()) {
        cerr << "Error: File not able to be opened" << endl;
        return;
    }

    json << "{";
    json << "    \"round\":" << round_number << "," << endl;
    //json << "    \"\":\""<< << "\"," << endl;
    json << "    \"Environment\":"<< this->env_id<< "," << endl;
    json << "    \"save time\":"<< time(NULL) << "," << endl;
    json << "    \"flock_size\":"<< flockSize<< "," << endl;
    json << "    \"frames\":"<< this->runTime << "," << endl;
    json << "    \"mutation rate\":"<< mutate_rate << "," << endl;


    //Output the population
    json << "    \"population\":[" << endl;
    for(unsigned int i=0; i<flockSize; i++) {
        json << "        {" << endl;
        json << "            \"proj_weight\":"<< pop[i].values.proj_weight << "," << endl;
        json << "            \"align_weight\":"<< pop[i].values.align_weight << "," << endl;
        json << "            \"noise_weight\":"<< pop[i].values.noise_weight << "," << endl;
        json << "            \"score\":"<< pop[i].score << "," << endl;

        json << "            \"source\":\"";
        switch(pop[i].source_info) {
            case 0:
                json << "copy";
                break;
            case 1:
                json << "mutate";
                break;
            case 69:
                json << "json";
                break;
            default:
                json << "unknown";
                break;
        }

        json << "\"" << endl << "        }";
        if(i != flockSize-1) json << ",";
        json << endl;
    }
    json << "    ]" << endl;
    json << "}";
    json.close();
}

/*
 * Takes as a argument a path to a json output file from a previous run
 * of the GA. It then initiates the global variables with the correct
 * values for the GA run AND sets pop and pop_new (which are args)
 * ready to continue the work of the GA
*/
void Simulation::load_json(string filename) {
    char *a=new char[filename.size()+1];
    a[filename.size()]=0;
    memcpy(a,filename.c_str(),filename.size());


    FILE * pFile = fopen (a, "r");
    rapidjson::FileStream is(pFile);
    rapidjson::Document document;
    document.ParseStream<0>(is);

    cout << "DONE! Round " << document["round"].GetInt() << endl;

    env_id = document["Environment"].GetInt();
    //flockSize = document["flock_size"].GetInt();
    //runTime = document["frames"].GetInt()+1;
    mutate_rate = document["mutation rate"].GetDouble();

    // Init the population
    for (rapidjson::SizeType i = 0; i < document["population"].Size(); i++) {
        
        agents[i].values.proj_weight = document["population"][i]["proj_weight"].GetDouble();
        agents[i].values.align_weight = document["population"][i]["align_weight"].GetDouble();
        agents[i].values.noise_weight = 1.0 - (agents[i].values.proj_weight+agents[i].values.align_weight);
        agents[i].score = document["population"][i]["score"].GetDouble();
        agents[i].source_info = 22;
        cout << "Loading agent "<< i <<"(" << agents[i].values.proj_weight << "," <<agents[i].values.align_weight << endl;
    }
}

Simulation::Simulation(int flockSize, SwarmValues *values) {
    this->flockSize = flockSize;
    agents = new Agent[flockSize];
    display = 0;
    this->values = *values;
    this->shape_dif = 0;
    this->speed = 4.3;
    this->raw_size = 10.;
    this->shape_ratio = 10;
    this->env_id = -1;
    json_dir = NULL;
    this->evolve = false;
    this->one_direction = false;
    this->round_length = 300;
    this->onFrame = 0;
    this->env = 0;
    setup_fast_math();
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

void Simulation::setEnvironment(Environment *env) {
    this->env_id = env->id;
    this->onFrame = env->onFrame;
    this->env = env;
    if(display != 0) {
        display->setOnDrawFunction(env->onDraw);
    }
}

void Simulation::setSwarmValues(SwarmValues *values) {
    this->values = *values;
    for(unsigned int i=(flockSize-1); i>0; i--) {
        agents[i].values = *values;
    }
}

//TODO test
void Simulation::getKNN(const Point2d p, Agent *knn[], const int number, const unsigned int ignore_index) {
    std::vector<double> knn_distance;
    for(int i=0; i<number;i++) knn_distance.push_back(-1);

    double largest_distance = -1.0;
    int largest_index = -1;
    int filled = 0;
    
    for(unsigned int i=(flockSize-1); i>0; i--) {
        if(i == ignore_index) continue;

        if(filled < number) {
            //Automatically accept it as the array isn't even full yet
            knn[filled] = &(this->agents[i]);
            knn_distance[filled] = knn[filled]->distanceFrom(&p);
            if(largest_distance <0 || knn_distance[filled] > largest_distance) {
                largest_distance = knn_distance[filled];
                largest_index = filled;
            }
            filled++;
        } else {

            double distance = agents[i].distanceFrom(&p);
            // If this should be put into our knn array
            if(distance < largest_distance) {
                //Find which one we should replace
                knn[largest_index] = &(agents[i]);
                knn_distance[largest_index] = distance;
                largest_distance = -1;
                largest_index = -1;
                for(int k=0; k<number; k++) {
                    if(knn_distance[k] > largest_distance) {
                        largest_distance = knn_distance[k];
                        largest_index = k;
                    }
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
        theta = atan2(from.getY(), from.getX());

        double apear_size = getApearSize(i,j);
        double size_angle = fastatan(apear_size/(from.getMagnitude()));
        // The number of bins the agent fills (half as based on radius)
        int size_bins = (int)(size_angle/binSize);
        if(size_bins ==0) size_bins = 1;

        int bin_N = (int)((theta)/binSize);
        //if(bin_N <= 0) bin_N = 1;
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

void Simulation::combine_vectors(Vector2d &current, Vector2d &prefered) {
    current *= 5;
    current.setX(current.getX() + prefered.getX());
    current.setY(current.getY() + prefered.getY());

   // current.setX(prefered.getX());
   // current.setY(prefered.getY());
}

void Simulation::runSimulation(const long maxRunTime) {
    this->maxRunTime = maxRunTime;
    this->runTime = 0;

    //Make the bins for finding the projection
    std::vector<char> bin;
    for(int j=0; j<BIN_COUNT;j++) bin.push_back(false);

    Agent **knn = new Agent*[4];
    Point2d center;
    getCenterOfMass(&center);

    if(this->env->init != NULL) this->env->init();
    if(this->env->roundStart != NULL) this->env->roundStart(this);

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
                v_neig *= agents[i].values.align_weight;
                v_rand *= agents[i].values.noise_weight;
                v_proj *= agents[i].values.proj_weight;

                // Sum all vectors into the v_neig
                v_neig.setX(v_neig.getX() + v_rand.getX());
                v_neig.setY(v_neig.getY() + v_rand.getY());

                v_neig.setX(v_neig.getX() + v_proj.getX());
                v_neig.setY(v_neig.getY() + v_proj.getY());

                //Fix speed
                if(v_neig.getMagnitude()>0)
                    v_neig /= v_neig.getMagnitude();
                v_neig *= speed;

                Vector2d old_velocity = agents[i].getVelocity();


                combine_vectors(old_velocity, v_neig);

                //Fix speed (again)
                if(old_velocity.getMagnitude()>0)
                    old_velocity /= old_velocity.getMagnitude();
                old_velocity *= speed;

                //agents[i].updateVelocity(&v_neig);
                agents[i].updateVelocity(&old_velocity);
                agents[i].updateLocation();
            }

            //TODO Move this into own function? (It's so awful)
            if(evolve && (this->getRunTime()%round_length)==0) {

                if(this->env->roundEnd != NULL) this->env->roundEnd(this);

                // Revert any bad mutations from the last round
                for(int bad_i = (flockSize/10)*9; bad_i>=(flockSize/10); bad_i=bad_i-1) {
                    agents[bad_i].revertValues();
                }

                //Sort worst first
                sort(agents, agents+flockSize);

                cout << "#### Best params (" << agents[flockSize-1].values.align_weight << "," 
                    << agents[flockSize-1].values.proj_weight << ") @ "
                    << agents[flockSize-1].score
                    << " on round " << (this->getRunTime()/round_length) << endl;

                for(int bad_i = (flockSize/10)*9; bad_i>=(flockSize/10); bad_i=bad_i-1) {
                    agents[bad_i].tryValues(
                            mutate_f(agents[bad_i].values.proj_weight), 
                            mutate_f(agents[bad_i].values.align_weight)
                        );
                }

                for(int bad_i = flockSize/10; bad_i>=0; bad_i=bad_i-1) {
                    agents[bad_i].setLocation((agents[flockSize-bad_i-1].getLocationX()),
                        (agents[flockSize-bad_i-1].getLocationY()));
                    do{
                        agents[bad_i].values.align_weight = mutate_f(agents[flockSize-bad_i-1].values.align_weight);
                        agents[bad_i].values.proj_weight = mutate_f(agents[flockSize-bad_i-1].values.proj_weight);

                        //Fix invalid values
                        if(agents[bad_i].values.align_weight<0) agents[bad_i].values.align_weight = 0.001;
                        else if(agents[bad_i].values.align_weight>1.0) agents[bad_i].values.align_weight = 0.998;
                        if(agents[bad_i].values.proj_weight<0) agents[bad_i].values.proj_weight = 0.001;
                        else if(agents[bad_i].values.proj_weight>1.0) agents[bad_i].values.proj_weight = 0.998;
                    } while(agents[bad_i].values.align_weight+agents[bad_i].values.proj_weight >= 1.0);
                    agents[bad_i].values.noise_weight = 1.0 - (agents[bad_i].values.align_weight+agents[bad_i].values.proj_weight);
                }

                // Save the round data (if a dir is set)
                if(json_dir != NULL) {
                    save_round((this->getRunTime()/round_length), agents, flockSize);
                }

                for(int bad_i = flockSize-1; bad_i>=0; bad_i=bad_i-1) {
                    agents[bad_i].score = 0;
                }

                if(this->env->roundStart != NULL) this->env->roundStart(this);

            }

        } else {
            // If a skip frame then don't change the velocity just move each
            // agent in the direction they were going
            for(unsigned int i=0; i<flockSize; i++) {
                agents[i].updateLocation();
            }
        }
        if(this->onFrame != 0) {
            this->onFrame(this);
        }

        // If there is a display then draw it
        if(/*runTime > 1500 && */display != 0) {
            display->drawDisplay();
        }

        this->runTime++;
    }

    if(this->env->roundEnd != NULL) this->env->roundEnd(this);
    if(this->env->destroy != NULL) this->env->destroy();

    delete knn;
}

void Simulation::reset() {
    this->setScore(0);

    double spread = 40;//100.0;
    Vector2d *v = new Vector2d();
    for(unsigned int i=(flockSize-1); i>0; i--) {
        spread = ((double)rand()/(double)RAND_MAX)*50 + 50;
        double x = ((double)rand()/(double)RAND_MAX)*spread-(spread/2);
        //double y = ((double)rand()/(double)RAND_MAX)*spread;
        double y = sqrt(((spread/2)*(spread/2))-((x)*(x)));
        if( (rand()&1) == 0) y= -y;

        if(one_direction) {
            v->setX(1);
            v->setY(0);
        } else {
            agents[i].setLocation(
                    x, 
                    y
                );
            double d = ((double)rand()/(double)RAND_MAX)*2.0 - 1.0;
            //double d = x>50 ? (y>50? 0.5 : 0.99) : (y<50? -0.5 : -0.99);

            //Set Y so speed = 1
            double y_val = fastsqrt(1.0 - d*d);
            if( (rand()&1) == 0) {
                y_val = -y_val;
            }    
            v->setX(d);
            v->setY(y_val);
        }



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

void Simulation::incScore(int v) {
    score += v;
}

long long Simulation::getScore() {
    return score;
}

void Simulation::setScore(long long score) {
    this->score = score;
}

void Simulation::setEvolve(bool e) {
    this->evolve = e;
}

void Simulation::setOneDirection(bool one_direction) {
    this->one_direction = one_direction;
}

int Simulation::getRoundLength(){
    return this->round_length;
}

