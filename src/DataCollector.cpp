using namespace std;
#include <iostream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include <math.h>
#include "Vector2d.h"
#include "Simulation.h"
#include "Display.h"
#include "Environment.h"
#include "EnvironmentStore.h"

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
    SwarmValues *v = new SwarmValues();
    v->proj_weight = 0;
    v->align_weight = 0;
    v->noise_weight = 1-v->proj_weight-v->align_weight;

    environment_center_disp_setMinimise(false);
    Environment *env = new Environment();
    env->onDraw = 0;
    env->onFrame = 0;
    env->onFrame = &environment_center_disp_onFrame;


/*
    //environment_food_init(200);
    Environment *env = new Environment();
    env->onDraw = &environment_displacement_onDraw;
    env->onFrame = &environment_displacement_onFrame;
*/
    Simulation *s = new Simulation(100, v);
    s->reset();
    s->setEnvironment(env); //uncomment this to add an environment

    s->setHillClimb(false);
    s->setEvolve(false);

    //s->runSimulation(10000);

    double i, j;
    int k, grid_i;
    double step = 0.049;
    #define SAMPLE_NUMBER 10
    #define GRID_NUMBER 1
    #define RUN_TIME 3000

    float progress = 0.0;
    float progress_step_size = 1.0/(float)( ((1/step)*(1/step))/2 * GRID_NUMBER);
    //                                      ^Points in a grid       ^Number of grids 

    long start_time = time(NULL);

    cerr << endl;
    cerr << "                                                                   \r";

    cout << "[" << endl;
    for(grid_i=GRID_NUMBER; grid_i>0; grid_i--) {
        cout << "  [" << endl; //Grid start
        for(i=0.0; i<1.0; i+=step) {
            cout << "    [" << endl; //Row start
            for(j=0.0; j<1.0; j+=step) {
                long long value_a[SAMPLE_NUMBER] = {0};
                if(i+j<=1.0) {
                    //long runCount = 0;
                    for(k=0; k<SAMPLE_NUMBER; k++) {
                        s->reset();
                        srand(time(NULL)+k*7+i*13+j*21);
                        v->proj_weight = i;
                        v->align_weight = j;
                        v->noise_weight = 1 - v->proj_weight - v->align_weight;
                        s->setSwarmValues(v);
  
                        s->setScore(0);
                        s->runSimulation(RUN_TIME);
                        environment_food_destroy();
                        //if(s->getScore() >=0) {
                           // runCount++;
                            value_a[k] = s->getScore();
                        //}
                        //environment_displacement_destroy();
                    }
                    sort(value_a, value_a+SAMPLE_NUMBER);
                    cout << "      ["; //COL start
                    for(int a=0; a<SAMPLE_NUMBER; a++) {
                        cout<<value_a[a];
                        if(a != SAMPLE_NUMBER-1) cout << ",";
                    }
                    cout << "]," << endl; //COL end

                    //Give an indication of the progress so far
                    progress += progress_step_size;

                    long time_taken = time(NULL)-start_time;
                    long total_time_prediction = time_taken * 1/progress;

                    // Carriage return (\r) makes it re-write the line again
                    // with the up to date information
                    cerr << "\r Progress: ";

                    fprintf(stderr, "%5.2f", progress*100 );
                    cerr << "%   Remaining: " 
                            << ((total_time_prediction-time_taken)/60.0)
                            << "min       \r";
                } else {
                    cout << "      []," << endl;
                }
                //cout << value << " ";
            }
            cout << "      []" << endl; //To fix the floating comma in the row
            cout << "    ]," << endl; //ROW end
        }
        cout << "    []" << endl; //To fix the floating comma in the column
        cout << "  ]" << endl; // end GRID
        if(grid_i != 0) cout << ",";
    }
    cout << "]" << endl;

    //Print info on the time taken
    cerr << endl << endl;
    long time_taken = time(NULL)-start_time;
    int hours = floor(time_taken/(60*60));
    int mins = floor((time_taken%60)/(60));
    int secs = floor((time_taken%(60*60)));
    cerr << "Time taken: "
            << hours << "h "
            << mins  << "m "
            << secs   << "s"
            << "  (" << time_taken << "s)"
            << endl;

    delete s;
    delete v;
    delete env;
}