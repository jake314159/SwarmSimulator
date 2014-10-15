using namespace std;
#include <iostream>
#include "Vector2d.h"
#include "Simulation.h"
#include "Display.h"

int main() {
    cout << "This will be the entry point to the program" << endl;

    SwarmValues *v = new SwarmValues();
    v->proj_weight = 0.0;
    v->align_weight = 0.8;
    v->noise_weight = 0.2;

    Simulation *s = new Simulation(100, v);

    s->reset();
    s->addDisplay();
    s->runSimulation(10000);

    delete s;
    delete v;
}
