using namespace std;
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <algorithm>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/writer.h"
#include "rapidjson/filestream.h"

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

int compare(char* string1, char* string2) 
{
    int i = 0;
    while(string1[i] != '\0' && string2[i] != '\0') {
        if(string1[i] != string2[i]) {
            if(string1[i] < string2[i]) {
                return 1;
            } else {
                return -1;
            }
        }
        i++;
    }
    return 0; //0 for equal
}

// Important values
int flock_size = 50;
int frame_number = 1000;

int sample_number = 7;
//Which quartile to use as the score
int sample_method = 2;
double mutation_rate = 0.03;

int population_size = 15;

int roundN = 0;
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

/*
 * Takes as a argument a path to a json output file from a previous run
 * of the GA. It then initiates the global variables with the correct
 * values for the GA run AND sets pop and pop_new (which are args)
 * ready to continue the work of the GA
*/
void load_json(string filename, std::vector<POPULATION> &pop, 
					std::vector<POPULATION> &pop_new) {
	char *a=new char[filename.size()+1];
	a[filename.size()]=0;
	memcpy(a,filename.c_str(),filename.size());


	FILE * pFile = fopen (a, "r");
	rapidjson::FileStream is(pFile);
	rapidjson::Document document;
	document.ParseStream<0>(is);

	cout << "DONE! Round " << document["round"].GetInt() << endl;

	roundN = document["round"].GetInt()+1;
	env_id = document["Environment"].GetInt();
	sample_number = document["sample number"].GetInt();
	flock_size = document["flock_size"].GetInt();
	frame_number = document["frames"].GetInt();
	mutation_rate = document["mutation rate"].GetDouble();

	// Init the population
	for (rapidjson::SizeType i = 0; i < document["population"].Size(); i++) {

		POPULATION p;

		p.proj_w = document["population"][i]["proj_weight"].GetDouble();
		p.align_w = document["population"][i]["noise_weight"].GetDouble();
		p.noise_w = 1.0 - (p.proj_w+p.align_w);
		p.score = document["population"][i]["score"].GetDouble();
		p.time_tested = 0;
		p.time_taken = 0;
		p.source_info = 69;
		pop.push_back(p);
		pop_new.push_back(p);
	}

}

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
	json << "    \"round\":" << round_number << "," << endl;
	//json << "    \"\":\""<< << "\"," << endl;
	json << "    \"Environment\":"<< env_id<< "," << endl;
	json << "    \"save time\":"<< time(NULL) << "," << endl;
	json << "    \"sample number\":"<< sample_number<< "\"," << endl;
	json << "    \"flock_size\":"<< flock_size<< "," << endl;
	json << "    \"frames:"<< frame_number << "," << endl;
	json << "    \"mutation rate\":"<< mutation_rate << "," << endl;


	//Output the population
	json << "    \"population\":[" << endl;
	for(unsigned int i=0; i<pop->size(); i++) {
		json << "        {" << endl;
		json << "            \"proj_weight\":"<< (*pop)[i].proj_w << "," << endl;
		json << "            \"align_weight\":"<< (*pop)[i].align_w << "," << endl;
		json << "            \"noise_weight\":"<< (*pop)[i].noise_w << "," << endl;
		json << "            \"score\":"<< (*pop)[i].score << "," << endl;

		json << "            \"source\":\"";
		switch((*pop)[i].source_info) {
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
		json << "\"," << endl;

		json << "            \"seed\":"<< (*pop)[i].seed << "" << endl;
		json << "        }";
		if(i != pop->size()-1) json << ",";
		json << endl;
	}
	json << "    ]" << endl;
	json << "}";
	json.close();
}

//TODO still uses variables visible globally (when it really doesn't need too)
long long fitness_function(Simulation *s, int run_time) {
   	std::vector<long long> score_store;
	for(int sample=0; sample<sample_number; sample++) {
        
        s->reset();
        if(env.init != NULL)
        	env.init();

        s->runSimulation(run_time);
        
        if(env.destroy != NULL)
        	env.destroy();

        score_store.push_back(s->getScore());
    }
    //cout << endl;
    sort(score_store.begin(), score_store.end());

    return score_store[sample_method];
}

int main(int argc, char *argv[]) {
	json_dir = argv[1];
	char * cont_json = 0;

	if(argc>3 && !compare(argv[2], "--continue")) {
		cont_json = argv[3];
	}

	SwarmValues *v = new SwarmValues();
    v->proj_weight = 0.4;
    v->align_weight = 0.4;
    v->noise_weight = 0.2;

   	std::vector<POPULATION> pop;
   	std::vector<POPULATION> pop_new;

   	std::vector<long long> score_store;
   	std::vector<unsigned int> seed_store;

	int i;
   	if(cont_json != NULL) {
		load_json(cont_json, pop, pop_new);
	} else {
	   	// Init the population
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
	}

	for(i=0; i<sample_number; i++) {
		score_store.push_back(-1);
		seed_store.push_back(-1);
	}

	switch(env_id) {
		case 1:
			//FOOD
            env.onDraw = &environment_food_onDraw;
            env.onFrame = &environment_food_onFrame;
            env.init = &environment_food_init_no_arg;
            env.destroy = &environment_food_destroy;
			break;
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

	        pop_new[i].score = fitness_function(s, frame_number);
	        pop_new[i].seed = 0; //Seed is no longer used

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
	        }
    	}

    	pop = pop_new;


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