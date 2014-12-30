using namespace std;
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include "Vector2d.h"
#include "Simulation.h"
#include "Environment.h"
#include "EnvironmentStore.h"

typedef struct {
	double proj_w, align_w, noise_w;
	long long score;
	//long long all_scores[] 
	long time_tested, time_taken;

	// Seed used to find score
	// used to replay a score record
	unsigned int seed;

	// Used to store info on the source of the individual
	// Shouldn't be recorded in this form
	int source_info;
} POPULATION;

// Important values
int flock_size = 50;
int frame_number = 1000;

int sample_number = 7;
//Which quartile to use as the score
int sample_method = 2;
double mutation_rate = 0.03;

int population_size = 15;

int round_limit = 100;

Environment env;
int env_id = 1;

char* json_dir = NULL;

void environment_food_init_no_arg(void) {
	// Init with a constant amount of food
	environment_food_init(200);
}

double rand_f() {
	return static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
}
bool pop_sort_function(POPULATION i, POPULATION j) { return i.score > j.score; }


void save_round(int round_number, std::vector<POPULATION> *pop) {
	char buff[200];

	sprintf(buff, "%s/%06d.json", json_dir, round_number);
	cout << "Write to '" << buff << "'" << endl;

	ofstream json;
	json.open (buff);

	if (!json.is_open()) {
		cerr << "Error: File not able to be opened" << endl;
		return;
	}

	json << "{";
	json << "    \"round\":\"" << round_number << "\"," << endl;
	//json << "    \"\":\""<< << "\"," << endl;
	json << "    \"Environment\":\""<< env_id<< "\"," << endl;
	json << "    \"save time\":\""<< time(NULL) << "\"," << endl;
	json << "    \"sample number\":\""<< sample_number<< "\"," << endl;
	json << "    \"flock_size\":\""<< flock_size<< "\"," << endl;
	json << "    \"frames\":\""<< frame_number << "\"," << endl;
	json << "    \"mutation rate\":\""<< mutation_rate << "\"," << endl;


	//Output the population
	json << "    \"population\":[" << endl;
	for(unsigned int i=0; i<pop->size(); i++) {
		json << "        {" << endl;
		json << "            \"proj_weight\":\""<< (*pop)[i].proj_w << "\"," << endl;
		json << "            \"align_weight\":\""<< (*pop)[i].align_w << "\"," << endl;
		json << "            \"noise_weight\":\""<< (*pop)[i].noise_w << "\"," << endl;
		json << "            \"score\":\""<< (*pop)[i].score << "\"," << endl;

		json << "            \"source\":\"";
		switch((*pop)[i].source_info) {
			case 0:
				json << "copy";
				break;
			case 1:
				json << "mutate";
				break;
			default:
				json << "unknown";
				break;
		}
		json << "\"," << endl;

		json << "            \"seed\":\""<< (*pop)[i].seed << "\"" << endl;
		json << "        }";
		if(i != pop->size()-1) json << ",";
		json << endl;
	}
	json << "    ]" << endl;
	json << "}";
	json.close();
}

int main(int argc, char *argv[]) {
	json_dir = argv[1];

	int roundN = 0;

	SwarmValues *v = new SwarmValues();
    v->proj_weight = 0.4;
    v->align_weight = 0.4;
    v->noise_weight = 0.2;

   	std::vector<POPULATION> pop;
   	std::vector<POPULATION> pop_new;

   	std::vector<long long> score_store;
   	std::vector<unsigned int> seed_store;

	switch(env_id) {
		case 1:
			//FOOD
            env.onDraw = &environment_food_onDraw;
            env.onFrame = &environment_food_onFrame;
            env.init = &environment_food_init_no_arg;
            env.destroy = &environment_food_destroy;
			break;
	}


	// Init the population
	int i;
	for(i=0; i<population_size; i++) {
		POPULATION p;
		// Loop until random weights are valid
		do {
			p.proj_w = rand_f();
			p.align_w = rand_f();
		} while((p.proj_w+p.align_w)>1.0);
		p.noise_w = 1.0 - (p.proj_w+p.align_w);
		p.score = 0;
		p.time_tested = 0;
		p.time_taken = 0;
		p.source_info = -1;
		pop.push_back(p);
		pop_new.push_back(p);
	}

	for(i=0; i<sample_number; i++) {
		score_store.push_back(-1);
		seed_store.push_back(-1);
	}

	Simulation *s = new Simulation(flock_size, v);
    s->reset();
    s->setEnvironment(&env); //uncomment this to add an environment

    for(;roundN<=round_limit;roundN++) {
    	cout << "Begin round " << roundN << " with a pop size of " << pop.size() << endl;
    	unsigned int i;
    	for(i=0; i<pop.size(); i++) {

    		// Create the new population
    		char success = false;
    		do {
	    		//double delta = (rand_f()/(mutation_rate*2))-mutation_rate;
	    		//double delta = ((rand()&1) ? mutation_rate : -mutation_rate);// * (1-(roundN/(round_limit+1)));
	    		double delta = ((rand()&1) ? mutation_rate : -mutation_rate) 
	    						+ ((rand()&1) ? (mutation_rate/5)*rand_f() : (-mutation_rate/5))*rand_f();
	    		if((rand() & 1) == 0) {
		    		pop_new[i].proj_w  = pop[i].proj_w + delta;
				    pop_new[i].align_w = pop[i].align_w;
				    if(pop_new[i].proj_w<0 || pop_new[i].align_w<0
				    		|| pop_new[i].proj_w+pop_new[i].align_w > 1.0) {
				    	pop_new[i].proj_w -= delta;
				    } else {
				    	success = true;
				    }
				} else {
		    		pop_new[i].proj_w  = pop[i].proj_w;
				    pop_new[i].align_w = pop[i].align_w + delta;
				    if(pop_new[i].proj_w<0 || pop_new[i].align_w<0 
				    		|| pop_new[i].proj_w+pop_new[i].align_w > 1.0) {
				    	pop_new[i].align_w -= delta;
				    } else {
				    	success = true;
				    }
				}
			} while(!success);
			pop_new[i].noise_w = 1.0 - (pop_new[i].proj_w + pop_new[i].align_w);


		    v->proj_weight = pop_new[i].proj_w;
		    v->align_weight = pop_new[i].align_w;
		    v->noise_weight = pop_new[i].noise_w;
			s->setSwarmValues(v);

			for(int sample=0; sample<sample_number; sample++) {

				unsigned int seed = time(NULL) ^ (roundN<<24) ^ (i<<16) ^ (sample<<8);
		        srand(seed);
		        s->reset();
		        if(env.init != NULL)
		        	env.init();
		        s->setScore(0);
	            s->runSimulation(frame_number);
	            if(env.destroy != NULL)
	            	env.destroy();

	            score_store[sample] = s->getScore();
	            //cout << "" << score_store[sample] << ", ";
	            seed_store[sample] = seed;
	        }
	        //cout << endl;
	        sort(score_store.begin(), score_store.end());

            pop_new[i].score = score_store[sample_method];
            //TODO seed is incorrect!
            pop_new[i].seed = seed_store[sample_method];
            pop_new[i].source_info = 1; //mutate

            if(pop_new[i].score < pop[i].score) {
	        	//Rollback
	    		pop_new[i].proj_w  = pop[i].proj_w;
			    pop_new[i].align_w = pop[i].align_w;
			    pop_new[i].noise_w = pop[i].noise_w;

			    pop_new[i].seed = pop[i].seed;
			    pop_new[i].source_info = 0; //copy
			    pop_new[i].score = pop[i].score;

    		    v->proj_weight = pop_new[i].proj_w;
			    v->align_weight = pop_new[i].align_w;
			    v->noise_weight = pop_new[i].noise_w;
				s->setSwarmValues(v);

			    // Re-score the roll backed version
				for(int sample=0; sample<sample_number; sample++) {

					unsigned int seed = time(NULL) ^ (roundN<<24) ^ (i<<16) ^ (sample<<8) ^ (1<<30);
			        srand(seed);
			        s->reset();
			        if(env.init != NULL)
			        	env.init();
			        s->setScore(0);
		            s->runSimulation(frame_number);
		            if(env.destroy != NULL)
		            	env.destroy();

		            score_store[sample] = s->getScore();
		            seed_store[sample] = seed;
		        }
		        sort(score_store.begin(), score_store.end());
		        pop_new[i].score = (pop[i].score*9+score_store[sample_method])/10;
		        //cout << pop[i].score << " and " <<score_store[sample_method] << " makes " << pop_new[i].score << endl;
	        }

            //cout << "Score: " << pop_new[i].score << " with seed\t" << pop_new[i].seed << endl;
    	}

    	pop = pop_new;
		/*for(i=0; i<pop.size(); i++) {
    		pop[i].proj_w  = pop_new[i].proj_w;
		    pop[i].align_w = pop_new[i].align_w;
		    pop[i].noise_w = pop_new[i].noise_w;
		    pop[i].score = pop_new[i].score;
		    pop[i].seed = pop_new[i].seed;
		    pop[i].source_info = pop_new[i].source_info;
		}*/
		//sort(pop.begin(), pop.end(), pop_sort_function);

    	long long best_score = 0;
    	int best_index = 0;
    	for(i=0; i<pop.size(); i++) {
    		if(pop[i].score > best_score) {
    			best_score = pop[i].score;
    			best_index = i;
    		}
    	}
    	cout << "Best score: " << best_score << " (" << pop[best_index].proj_w 
    		<< "," << pop[best_index].align_w << ")" << endl;
		save_round(roundN, &pop);
    }
}