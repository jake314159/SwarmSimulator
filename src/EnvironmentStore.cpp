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
double spread = 800;
FOOD *food;
void environment_food_init(int numberOfFood) {
    int i;
    foodCount = numberOfFood;
    food = (FOOD*)malloc(sizeof(FOOD)*foodCount);
    for(i=0; i<foodCount; i++) {
        food[i].x = ((double)rand()/(double)RAND_MAX)*spread - spread/2;
        food[i].y = ((double)rand()/(double)RAND_MAX)*spread - spread/2;
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
    unsigned int i,j;
    for(i=0; i<s->flockSize; i++) {
        for(j=0; j<foodCount; j++) {
            if( ((agents[i].getLocationX()-food[j].x)*(agents[i].getLocationX()-food[j].x)+
                (agents[i].getLocationY()-food[j].y)*(agents[i].getLocationY()-food[j].y)) <3
                ) {
                s->incScore(1);
                food[j].x = ((double)rand()/(double)RAND_MAX)*spread - spread/2;
                food[j].y = ((double)rand()/(double)RAND_MAX)*spread - spread/2;
            }
        }
    }
}

