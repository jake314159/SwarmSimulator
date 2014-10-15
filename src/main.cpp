using namespace std;
#include <iostream>
#include "Vector2d.h"
#include "Simulation.h"

int main() {
    cout << "This will be the entry point to the program" << endl;
    Simulation *s = new Simulation(100);
    s->reset();
    s->runSimulation(100);
    delete s;

}
