using namespace std;
#include <iostream>
#include <string>
#include <stdlib.h>
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

char check_for_float(char *s) {
    char decimalPoint = false;
    int i;
    for(i=0; s[i] != '\0'; i++) {
        switch(s[i]) {
            case '.':
                if(decimalPoint) return false;
                decimalPoint = true;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                break;
            default:
                return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    cout << "This will be the entry point to the program" << endl;

    bool have_display = true;
    bool allow_record = false;
    bool record_dir_set = false;
    std::string record_dir;

    if(!check_for_float(argv[1])) {
        cout << "Error: proj_w not a number" << endl;
        exit(44);
    }
    if(!check_for_float(argv[2])) {
        cout << "Error: align_w not a number" << endl;
        exit(44);
    }

    double proj_W_in = atof(argv[1]);
    double align_W_in = atof(argv[2]);
    if(proj_W_in <0) {
        cout << "Error: proj_W <0.0" << endl;
        exit(41);
    }
    if(align_W_in < 0) {
        cout << "Error: proj_W <0.0" << endl;
        exit(42);
    }
    if(proj_W_in + align_W_in >1.0) {
        cout << "Error: proj_W+align_W>1.0" << endl;
        exit(43);
    }
    double noise_W_in = 1.0 - (proj_W_in + align_W_in);

    // Go over other paramiters and update the settings if provided
    for(int i=3; i<argc; i++) {
        if(!compare(argv[i], "-R") || !compare(argv[i], "--EnableRecord")) {
            allow_record = true;
        } else if(!compare(argv[i], "-D")) {
            if(i == argc-1) cout << "Output dir not specified"<<endl;
            record_dir = argv[++i];
            record_dir_set = true;
        } if(!compare(argv[i], "--NoDisplay")) {
            have_display = false;
        } 
    }

    SwarmValues *v = new SwarmValues();
    v->proj_weight = proj_W_in;
    v->align_weight = align_W_in;
    v->noise_weight = noise_W_in;

    Simulation *s = new Simulation(100, v);
    s->reset();
    if(have_display) {
        s->addDisplay();

        if(allow_record && record_dir_set) {
            s->enableRecord(record_dir);
        }
    }

    s->runSimulation(10000000);

    delete s;
    delete v;
}
