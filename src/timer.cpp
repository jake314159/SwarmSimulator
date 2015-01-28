
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <algorithm>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/writer.h"   // for stringify JSON
#include "rapidjson/filestream.h"   // wrapper of C stream for prettywriter as output

#include "Vector2d.h"
#include "Simulation.h"
#include "Environment.h"
#include "EnvironmentStore.h"

using namespace std;

//Just run it once!
int sample_number = 1;
int sample_method = 0;

//TODO Copied from ga - import properly

long long fitness_function(Simulation *s, Environment &env, int run_time) {
   	std::vector<long long> score_store;
	for(int sample=0; sample<sample_number; sample++) {
        
        s->reset();
        if(env.init != NULL)
        	env.init();
        s->setScore(0);
        s->runSimulation(run_time);
        if(env.destroy != NULL)
        	env.destroy();
        long long score = s->getScore();
        score_store.push_back(score);
    }
    //cout << endl;
    sort(score_store.begin(), score_store.end());

    return score_store[sample_method];
}

//hack
void environment_food_init_no_arg(void) {
	// Init with a constant amount of food
	environment_food_init(200);
}

int main() {
	SwarmValues *v = new SwarmValues();
    v->proj_weight = 0.4;
    v->align_weight = 0.4;
    v->noise_weight = 0.2;

	Environment env;
    env.onDraw = &environment_food_onDraw;
    env.onFrame = &environment_food_onFrame;
    env.init = &environment_food_init_no_arg;
    env.destroy = &environment_food_destroy;

	Simulation *s = new Simulation(100, v);
	s->setEnvironment(&env);

	clock_t time_sum = 0;
	int count = 0;

	clock_t start;
	clock_t end;

	cout << "start of sum: " << time_sum << endl;
	while(true) {
		start = clock() / (CLOCKS_PER_SEC / 1000);
		cout << fitness_function(s, env, 1000);
		end = clock() / (CLOCKS_PER_SEC / 1000);
		clock_t time_taken = end-start;
		time_sum += time_taken;
		count++;
		cout << time_sum/count << "ms\t\t(N=" << count << ") +" << time_taken << endl;
	}
}
