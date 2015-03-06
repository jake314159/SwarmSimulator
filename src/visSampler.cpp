

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "Vector2d.h"
#include "Simulation.h"
#include "Display.h"
#include "Environment.h"
#include "EnvironmentStore.h"



int main(int argc, char *argv[]) {
    srand(time(NULL));

    if(argc <2) { 
    	cout << "Please specify the name of the json output file!" << endl;
    	exit(2);
    }

    ofstream json;
    json.open(argv[1]);

    if (!json.is_open()) {
        cerr << "Error: File not able to be opened" << endl;
        exit(3);
    }

	Environment *env = new Environment();
    env->onDraw = 0;
    env->id = 10;
    env->onFrame = &environment_scan_onFrame;

    //Not used but required to start the simulation
    SwarmValues *v = new SwarmValues();
    v->proj_weight = 0.45;
    v->align_weight = 0.45;
    v->noise_weight = 0.10;

    Simulation *s = new Simulation(100, v);

    s->setHillClimb(false);
    s->setEvolve(true);

    int roundNumber = 3000;
    long run_time = (s->getRoundLength() * roundNumber) + 1;

    s->setEnvironment(env);

    json << "[" << endl;

    int count = 0;
    double proj, align;

    for(int i=0; i<=50; i+=12) {
    	s->reset();
    	s->setVisibility((double)i);
    	s->runSimulation(run_time);
    	s->getAverageParamiter(&proj, &align);

    	json << "    {" << endl;
    	json << "        \"visibility\":" << i << "," << endl;
    	json << "        \"proj_w\":" << proj << "," << endl;
    	json << "        \"align_w\":" << align << "," << endl;
    	json << "        \"count\":" << ++count << endl;
    	json << "    }," << endl;
    }

    for(int i=55; i<=70; i+=5) {
    	s->reset();
    	s->setVisibility((double)i);
    	s->runSimulation(run_time);
    	s->getAverageParamiter(&proj, &align);

    	json << "    {" << endl;
    	json << "        \"visibility\":" << i << "," << endl;
    	json << "        \"proj_w\":" << proj << "," << endl;
    	json << "        \"align_w\":" << align << "," << endl;
    	json << "        \"count\":" << ++count << endl;
    	json << "    }," << endl;
    }

    for(int i=100; i<=200; i+=50) {
    	s->reset();
    	s->setVisibility((double)i);
    	s->runSimulation(run_time);
    	s->getAverageParamiter(&proj, &align);

    	json << "    {" << endl;
    	json << "        \"visibility\":" << i << "," << endl;
    	json << "        \"proj_w\":" << proj << "," << endl;
    	json << "        \"align_w\":" << align << "," << endl;
    	json << "        \"count\":" << ++count << endl;
    	json << "    }";
        if(i < 200) json << ",";
        json << endl;
    }

    json << "]" << endl;
}