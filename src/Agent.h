#ifndef AGENT_INCLUDE_GUARD_H
#define AGENT_INCLUDE_GUARD_H 1

#include "Point2d.h"
#include "Vector2d.h"

class Agent {
    private:
        Point2d location;
        Vector2d velocity;
    public:
        Agent();

        double getLocationX();
        double getLocationY();
        void updateVelocity(Vector2d *v);
        void updateLocation();
        double distanceFrom(Point2d *p);
        Vector2d* vectorFrom(Point2d *p);
};

#endif
