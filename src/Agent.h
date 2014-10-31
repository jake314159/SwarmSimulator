#ifndef AGENT_INCLUDE_GUARD_H
#define AGENT_INCLUDE_GUARD_H 1

#include "Point2d.h"
#include "Vector2d.h"

class Agent {
    private:
        Point2d location;
        Vector2d velocity;
        Vector2d velocity_next;
    public:
        Agent();

        void setLocation(double x, double y);
        double getLocationX();
        double getLocationY();
        void updateVelocity(Vector2d *v);
        void updateLocation();
        double distanceFrom(const Point2d *p);
        Vector2d* vectorFrom(Point2d *p, Vector2d *v);
        Vector2d getVelocity();
};

#endif
