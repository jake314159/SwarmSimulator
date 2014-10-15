using namespace std;
#include <iostream>
#include "Vector2d.h"
#include "Simulation.h"
#include "Display.h"

int main() {
    cout << "This will be the entry point to the program" << endl;
    Simulation *s = new Simulation(100);

    s->reset();
    s->addDisplay();
    s->runSimulation(10000);

    delete s;

}
