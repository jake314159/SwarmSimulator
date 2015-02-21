#include "Agent.h"
#include <stdlib.h>

double rand_f() {
    return static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
}

Agent::Agent () {
    source_info = -1; //Unknown by default
    do {
        this->values.proj_weight = rand_f();
        this->values.align_weight = rand_f();
    } while((this->values.proj_weight+this->values.align_weight)>1.0);
    this->values.noise_weight = 1.0 - (this->values.proj_weight+this->values.align_weight);

}

void Agent::updateVelocity(Vector2d *v){
    this->velocity_next.setVector(v);
}

void Agent::updateLocation() {
    this->velocity.setVector(&velocity_next);
    this->location.applyVector((this->velocity));
}

double Agent::distanceFrom(const Point2d *p) {
    return this->location.distanceFrom(*p);
}

Vector2d* Agent::vectorFrom(Point2d *p, Vector2d *v) {
    //Vector2d *v = new Vector2d(p->x-this->location.x, p->y-this->location.y);
    v->setX(p->x-this->location.x);
    v->setY(p->y-this->location.y);
    return v;
}

double Agent::getLocationX() {
    return this->location.x;
}

double Agent::getLocationY() {
    return this->location.y;
}

void Agent::setLocation(double x, double y) {
    this->location.x = x;
    this->location.y = y;
}

Vector2d Agent::getVelocity() {
    return (this->velocity);
}

void Agent::tryValues(double proj_w, double align_w) {
    //Fix invalid values
    if(align_w<0) align_w = 0.001;
    else if(align_w>1.0) align_w = 0.998;
    if(proj_w<0) proj_w = 0.001;
    else if(proj_w>1.0) proj_w = 0.998;

    //TODO Calculate exactly how much to reduce the weights
    //by and do it in one op
    while(proj_w+align_w > 1.0) {
        align_w *= 0.99;
        proj_w *= 0.99;
    }

    this->values_past.proj_weight = this->values.proj_weight;
    this->values_past.align_weight = this->values.align_weight;
    this->values_past.noise_weight = this->values.noise_weight;
    this->score_past = score;

    this->values.proj_weight = proj_w;
    this->values.align_weight = align_w;
    this->values.noise_weight = 1.0 - (proj_w + align_w);
}
void Agent::revertValues() {
    if(score_past > score) {
        this->values = this->values_past;
        this->score = this->score_past;
    }
}

void Agent::setVelocity(double x, double y) {
    velocity.setVector(x,y);
}