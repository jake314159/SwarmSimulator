#ifndef AGENT_INCLUDE_GUARD_H
#define AGENT_INCLUDE_GUARD_H 1

#include "Point2d.h"
#include "Vector2d.h"
#include "SwarmValues.h"

class Agent {
    private:
        Point2d location;
        Vector2d velocity;
        Vector2d velocity_next;
    public:
        Agent();

        SwarmValues values;
        long long score;
        SwarmValues values_past;
        long long score_past;
        void tryValues(double proj_w, double align_w);
        void revertValues();

        void setLocation(double x, double y);
        double getLocationX();
        double getLocationY();
        void updateVelocity(Vector2d *v);
        void updateLocation();
        double distanceFrom(const Point2d *p);
        Vector2d* vectorFrom(Point2d *p, Vector2d *v);
        Vector2d getVelocity();

        bool operator< (const Agent &other) const {
            return score < other.score;
        }
        bool operator> (const Agent &other) const {
            return score > other.score;
        }
};

#endif
