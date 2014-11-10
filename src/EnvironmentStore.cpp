using namespace std;
#include <stdlib.h>
#include <iostream>
#include "Display.h"
#include "Simulation.h"

/* Food */
typedef struct {
    double x, y;
} FOOD;

int foodCount;
double spread = 400;
FOOD *food;
void environment_food_init(int numberOfFood) {
    int i;
    foodCount = numberOfFood;
    food = (FOOD*)malloc(sizeof(FOOD)*foodCount);
    for(i=0; i<foodCount; i++) {
        food[i].x = ((double)rand()/(double)RAND_MAX)*spread*2 - spread;
        food[i].y = ((double)rand()/(double)RAND_MAX)*spread*2 - spread;
    }
}

void environment_food_destroy() {
    free(food);
}

void environment_food_onDraw(Display* d) {
    int i;
    for(i=0; i<foodCount; i++) {
        d->drawBox(food[i].x, food[i].y, 3, 255, 0, 0);
    }
}

void environment_food_onFrame(void *simulation) {
    Simulation *s = (Simulation*)simulation;
    Agent* agents = s->getAgents();
    Point2d p;
    s->getCenterOfMass(&p);
    unsigned int i,j;
    for(j=0; j<foodCount; j++) {
        double minDistance = -1;
        for(i=0; i<s->flockSize; i++) {
            double d = ((agents[i].getLocationX()-food[j].x)*(agents[i].getLocationX()-food[j].x)+
                (agents[i].getLocationY()-food[j].y)*(agents[i].getLocationY()-food[j].y));

            if(minDistance <0 || minDistance > d) {
                minDistance = d;
            }
            if( d<3 ) {
                s->incScore(1);
                food[j].x = ((double)rand()/(double)RAND_MAX)*spread - spread/2;
                food[j].y = ((double)rand()/(double)RAND_MAX)*spread - spread/2;
            }
        }

        //We've moved away from the food so lets move it closer
        if(minDistance > spread*spread) {
            food[j].x = ((double)rand()/(double)RAND_MAX)*spread*2 - spread + p.x;
            food[j].y = ((double)rand()/(double)RAND_MAX)*spread*2 - spread + p.y;
        }
    }
}

