#ifndef POINT2D_INCLUDE_GUARD_H
#define POINT2D_INCLUDE_GUARD_H 1

#include "Vector2d.h"

class Point2d {
    public:
        double x;
        double y;

        Point2d (double x, double y);
        Point2d ();

        void applyVector(Vector2d &v);
        double distanceFrom(const Point2d &p);
};

#endif
