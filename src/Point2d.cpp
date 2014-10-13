using namespace std;
#include <iostream>
#include "Point2d.h"

Point2d::Point2d () {
    this->x = 0;
    this->x = 0;
}

Point2d::Point2d (double x, double y) {
    this->x = x;
    this->y = y;
}

void Point2d::applyVector(Vector2d &v) {
    this->x += v.getX();
    this->y += v.getY();
}
