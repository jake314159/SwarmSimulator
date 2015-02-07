
#include <iostream>
#include <string>
#include <stdlib.h>
#include "Vector2d.h"
#include "Simulation.h"
#include "Display.h"
#include "Environment.h"
#include "EnvironmentStore.h"

using namespace std;

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

void myOnDraw(Display* d) {
    cout << "Display" << endl;
}
void myOnFrame(void *simulation) {
    cout << "FRAME!" << endl;
}

char checkForFloat(char *s) {
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
    srand(time(NULL));

    //A very large number by default
    int roundNumber = -1;
    long run_time = 1000000000;

    bool have_display = true;
    bool allow_record = false;
    bool record_dir_set = false;
    bool evolve = true;
    std::string record_dir;
    char* json_dir = NULL;
    char *start_json_file = NULL;

    if(argc < 3) {
        cout << "Error: Missing arguments" << endl;
        exit(40);
    }

    double proj_W_in = 0.0;
    double align_W_in = 0.0;
    double noise_W_in = 0.0;
    bool specify_params = true;
    if(argv[1][0] == 'X' || argv[2][0] == 'X') {
        specify_params = false;
    } else {

        if(!checkForFloat(argv[1])) {
            cout << "Error: proj_w not a number" << endl;
            exit(44);
        }
        if(!checkForFloat(argv[2])) {
            cout << "Error: align_w not a number" << endl;
            exit(44);
        }

        proj_W_in = atof(argv[1]);
        align_W_in = atof(argv[2]);
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
        noise_W_in = 1.0 - (proj_W_in + align_W_in);

    }

    Environment *env = new Environment();
    env->onDraw = 0;
    env->onFrame = 0;

    // Go over other parameters and update the settings if provided
    for(int i=3; i<argc; i++) {
        if(!compare(argv[i], "-R") || !compare(argv[i], "--EnableRecord")) {
            allow_record = true;
        } else if(!compare(argv[i], "--NoEvo")) {
            evolve = false;
        } else if(!compare(argv[i], "-D")) {
            if(i == argc-1) cout << "Output dir not specified"<<endl;
            record_dir = argv[++i];
            record_dir_set = true;
        } else if(!compare(argv[i], "--NoDisplay")) {
            have_display = false;
        } else if(!compare(argv[i], "-E")) {
            i++;
            /*if(!compare(argv[i], "FOOD")) {
                env->onDraw = &environment_food_onDraw;
                env->onFrame = &environment_food_onFrame;
            }*/
            if(!compare(argv[i], "SCAN")) {
                env->id = 10;
                env->onDraw = &environment_scan_onDraw;
                env->onFrame = &environment_scan_onFrame;
            } else if(!compare(argv[i], "FOOD")) {
                env->id = 11;
                env->onFrame = &environment_food_onFrame;
                env->roundStart = &environment_food_round_start;
            } else if(!compare(argv[i], "INTER")) {
                env->id = 12;
                env->onFrame = &environment_intersect_onFrame;
            } else if(!compare(argv[i], "SPREAD_MIN")) {
                env->id = 13;
                environment_spread_setMinimise(true);
                env->onFrame = &environment_spread_onFrame;
            } else if(!compare(argv[i], "SPREAD_MAX")) {
                env->id = 14;
                environment_spread_setMinimise(false);
                env->onFrame = &environment_spread_onFrame;
            } else if(!compare(argv[i], "M_DESC")) {
                env->id = 101;
                env->roundStart = &measure_describe_round_start;
                env->roundEnd = &measure_describe_round_end;
                env->init = &measure_describe_init;
                env->destroy = &measure_describe_destroy;
            } else {
                cout << "UNKOWN ENVIRONMENT!!" << endl;
                exit(199);
            }
        } else if(!compare(argv[i], "--RunTime")) {
            run_time = atoi(argv[++i]);
        } else if(!compare(argv[i], "--RoundN")) {
            roundNumber = atoi(argv[++i]);
        } else if(!compare(argv[i], "--jsonDir")) {
            json_dir = argv[++i];
        } else if(!compare(argv[i], "--startFromJson")) {
            start_json_file = argv[++i];
        }
    }

    SwarmValues *v = new SwarmValues();
    v->proj_weight = proj_W_in;
    v->align_weight = align_W_in;
    v->noise_weight = noise_W_in;

    //environment_food_init(200);

    Simulation *s = new Simulation(100, v);
    s->json_dir = json_dir;
    s->reset();

    if(specify_params) {
        s->setSwarmValues(v);
    }

    if(start_json_file != NULL) {
        s->load_json(start_json_file);
    }
    s->setEvolve(evolve);

    if(roundNumber>=0) {
        run_time = (s->getRoundLength() * roundNumber) + 1;
    }
    
    if(have_display) {
        s->addDisplay();

        if(allow_record && record_dir_set) {
            s->enableRecord(record_dir);
        }
    }
    s->setEnvironment(env); //uncomment this to add an environment

    s->runSimulation(run_time);
    cout << "Final score: " << s->getScore() << endl;

    delete s;
    delete v;
    delete env;
}
