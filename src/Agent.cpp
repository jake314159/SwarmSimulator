#include "Agent.h"

Agent::Agent () {
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
