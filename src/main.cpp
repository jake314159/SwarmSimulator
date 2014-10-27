using namespace std;
#include <iostream>
#include "Vector2d.h"
#include "Simulation.h"
#include "Display.h"

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

int main(int argc, char *argv[]) {
    cout << "This will be the entry point to the program" << endl;

    bool allow_record = false;
    bool record_dir_set = false;
    std::string record_dir;

    // Go over paramiters and update the settings if provided
    for(int i=1; i<argc; i++) {
        if(!compare(argv[i], "-R") || !compare(argv[i], "--EnableRecord")) {
            allow_record = true;
        } else if(!compare(argv[i], "-D")) {
            if(i == argc-1) cout << "Output dir not specified"<<endl;
            record_dir = argv[++i];
            record_dir_set = true;
        }
    }
    //cout << allow_record << " " << record_dir << endl;

    SwarmValues *v = new SwarmValues();

     //Fish
/*
    v->proj_weight = 0.4;
    v->align_weight = 0.4;
    v->noise_weight = 0.2;
    */

     // Birds 

    v->proj_weight = 0.1;
    v->align_weight = 0.75;
    v->noise_weight = 0.15;
    

     //Ants
/*    v->proj_weight = 0.1;
    v->align_weight = 0.4;
    v->noise_weight = 0.5;*/
    
 //other fish
/*    v->proj_weight = 0.4;
    v->align_weight = 0.4;
    v->noise_weight = 0.2;*/

    Simulation *s = new Simulation(100, v);
    s->reset();
    //s->addDisplay();

    if(allow_record && record_dir_set) {
    //    s->enableRecord(record_dir);
    }

    s->runSimulation(10000);

    delete s;
    delete v;
}
