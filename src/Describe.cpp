#include <iostream>
#include <string>
#include <stdlib.h>
#include "Vector2d.h"
#include "Simulation.h"
#include "Display.h"
#include "Environment.h"
#include "EnvironmentStore.h"


#define RUN_TIME 3500
#define SAMPLE_COUNT 20

int main(int argc, char *argv[]) {

	srand(time(NULL));

	//You should know what you are doing if you use this program
	//Note there are no checks on the paramiters

	double proj_W = atof(argv[1]);
    double align_W = atof(argv[2]);

    Environment *env = new Environment();
    env->id = 101;
    env->onFrame = &measure_describe_onFrame;
    env->roundStart = &measure_describe_round_start;
    env->roundEnd = &measure_describe_round_end;
    env->init = &measure_describe_init;
    env->destroy = &measure_describe_destroy;

    SwarmValues *v = new SwarmValues();
    v->proj_weight = proj_W;
    v->align_weight = align_W;
    v->noise_weight = 1-(proj_W+align_W);

    Simulation *s = new Simulation(100, v);

    s->setHillClimb(false);
    s->setEvolve(false);

    s->setSwarmValues(v);
    s->setEnvironment(env);

    double spread = 0;
    double speed = 0;
    double t_spread, t_speed;

    for(int i=0; i<SAMPLE_COUNT; i++) {
    	s->reset();
    	s->runSimulation(RUN_TIME);
    	measure_describe_get_vals(&t_spread, &t_speed);
    	spread += t_spread;
    	speed += t_speed;
    }
    spread /= SAMPLE_COUNT;
    speed /= SAMPLE_COUNT;

    cout << "$$$$ Paramiters ("<<proj_W<<","<<align_W<<") Have the desc values (spread,speed) of ("
    	<< spread << "," << speed << ")" << endl;
}