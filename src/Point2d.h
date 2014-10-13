#include "Vector2d.h"

class Point2d {
    public:
        double x;
        double y;

        Point2d (double x, double y);
        Point2d ();

        void applyVector(Vector2d &v);
};

