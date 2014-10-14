#include "../src/Point2d.cpp"
#include <gtest/gtest.h>

TEST(Point2d_test, constructor) { 
    double val_x = 8.0;
    double val_y = 11.0;
    
    Point2d *p = new Point2d(val_x, val_y);

    ASSERT_EQ(p->x, val_x);
    ASSERT_EQ(p->y, val_y);

    delete p;
}

TEST(Point2d_test, apply_vector) { 
    double val_x = 2.0;
    double val_y = 4.0;
    Vector2d *l = new Vector2d( val_x, val_y );
    
    Point2d *p = new Point2d(0, 0);

    p->applyVector(*l);
    p->applyVector(*l);

    ASSERT_EQ(p->x, val_x*2);
    ASSERT_EQ(p->y, val_y*2);

    delete l;
    delete p;
}
 
TEST(Point2d_test, distance) { 
    double answer = 5.0;
    
    Point2d *p1 = new Point2d(13.0, 6.0);
    Point2d *p2 = new Point2d(10.0, 2.0);

    ASSERT_EQ(p1->distanceFrom(*p2), answer);
    ASSERT_EQ(p2->distanceFrom(*p1), answer);

    delete p1;
    delete p2;
}
