#include "Agent.h"

Agent::Agent () {
}
void Agent::updateVelocity(Vector2d *v){
    this->velocity.setVector(v);
}
void Agent::updateLocation() {
    this->location.applyVector((this->velocity));
}
double Agent::distanceFrom(Point2d *p) {
    return this->location.distanceFrom(*p);
}
Vector2d* Agent::vectorFrom(Point2d *p) {
    Vector2d *v = new Vector2d(p->x-this->location.x, p->y-this->location.y);
    return v;
}

double Agent::getLocationX() {
    return this->location.x;
}

double Agent::getLocationY() {
    return this->location.y;
}
