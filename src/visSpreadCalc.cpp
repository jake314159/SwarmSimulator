
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
#include "rapidjson/stringbuffer.h"

#include "Vector2d.h"
#include "Simulation.h"
#include "Display.h"
#include "Environment.h"
#include "EnvironmentStore.h"

using namespace rapidjson;
using namespace std;

/*
 * Takes the output from visSampler.cpp as the first argument and outputs
 * the same file but with an added 'spread' paramiter in the json
 * to the file named after the second arg
 * eg.
 *     $ ./bin/VisSpreadCalc <input file> <output file>
 */

 #define RUN_TIME 3500
 #define SAMPLE_N 2

int main(int argc, char *argv[]) {
	//Parse the input json file
	FILE * pFile = fopen (argv[1], "r");
    rapidjson::FileStream is(pFile);
    rapidjson::Document document;
    document.ParseStream<0>(is);

    // Set up output stream
    ofstream json;
    json.open(argv[2]);

    if (!json.is_open()) {
        cerr << "Error: File not able to be opened" << endl;
        exit(3);
    }

    //Set up simulation
    Environment *env = new Environment();
    env->id = 101;
    env->onFrame = &measure_describe_onFrame;
    env->roundStart = &measure_describe_round_start;
    env->roundEnd = &measure_describe_round_end;
    env->init = &measure_describe_init;
    env->destroy = &measure_describe_destroy;

    SwarmValues *v = new SwarmValues();
    v->proj_weight = 0.45;
    v->align_weight = 0.45;
    v->noise_weight = 0.1;

    Simulation *s = new Simulation(100, v);

    s->setHillClimb(false);
    s->setEvolve(false);

    s->setEnvironment(env);

    double proj, align, vis, t_spread, t_speed;

    json << "[" << endl;

    for (rapidjson::SizeType i = 0; i < document.Size(); i++) {
    	proj = document[i]["proj_w"].GetDouble();
    	align = document[i]["align_w"].GetDouble();
    	vis = document[i]["visibility"].GetDouble();
    	cout << "Ok so the params are " << proj << " " << align << " with visibility " << vis <<endl;

    	json << "    {" << endl;
    	json << "        \"visibility\":" << vis << "," << endl;
    	json << "        \"proj_w\":" << proj << "," << endl;
    	json << "        \"align_w\":" << align << "," << endl;
    	json << "        \"count\":" << i+1 << "," << endl;

		SwarmValues *v = new SwarmValues();
	    v->proj_weight = proj;
	    v->align_weight = proj;
	    v->noise_weight = 1-(proj+align);
    	s->setSwarmValues(v);

    	json << "        \"spread\":[" << endl;
    	for(int j=0; j<SAMPLE_N; j++) {
	    	s->reset();
	    	s->runSimulation(RUN_TIME);
	    	measure_describe_get_vals(&t_spread, &t_speed);
	    	json << "            " << t_spread;
	    	if(j < SAMPLE_N-1) json << ",";
	    	json << endl;
		}
		json << "        ]" << endl;

    	json << "    }," << endl;
    }

    json << "{}" << endl;//Hacky fix to extra end comma

}
