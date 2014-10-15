using namespace std;
#include <iostream>
#include <math.h>
#include "Point2d.h"
#include "SwarmValues.h"

Point2d::Point2d () {
    this->x = 0;
    this->y = 0;
}

Point2d::Point2d (double x, double y) {
    this->x = x;
    this->y = y;
}

void Point2d::applyVector(Vector2d &v) {
    this->x += v.getX();
    this->y += v.getY();
}

double Point2d::distanceFrom(Point2d &p) {
    double a = p.x - this->x;
    double b = p.y - this->y;

    return sqrt (a*a + b*b);
}
