#include "../src/Agent.cpp"
#include <gtest/gtest.h>

/* Stub methods to convince the Agent to compile and run */
Point2d::Point2d(){}
Vector2d::Vector2d::Vector2d(){}
void Vector2d::setVector(Vector2d*){}

int apply_vector_count = 0;
void Point2d::applyVector(Vector2d&){
    apply_vector_count++;
}

const double DISTANCE_MAGIC_NUMBER = 123.4;
double Point2d::distanceFrom(Point2d &p) {
    return DISTANCE_MAGIC_NUMBER; // Some magic number
}

double fake_vector2d_x, fake_vector2d_y;
Vector2d::Vector2d::Vector2d(double x, double y){
    fake_vector2d_x = x;
    fake_vector2d_y = y;
}

Point2d::Point2d(double x, double y){
    this->x = x;
    this->y = y;
}

/* The tests! */
TEST(Agent_test, distance_from) { 
    
    Agent *a = new Agent();
    Point2d *p = new Point2d();
    double value = a->distanceFrom(p);
    ASSERT_EQ(value, DISTANCE_MAGIC_NUMBER);

    delete a;
    delete p;
}

TEST(Agent_test, update_location) { 
    
    Agent *a = new Agent();
    int cnt = apply_vector_count;
    a->updateLocation();
    ASSERT_EQ(apply_vector_count, cnt+1);

    delete a;
}

TEST(Agent_test, vector_from) { 
    double x=1.1, y=2.2;
    Agent *a = new Agent();
    Point2d *p = new Point2d(x,y);
    a->vectorFrom(p);
    ASSERT_EQ(fake_vector2d_x, x);
    ASSERT_EQ(fake_vector2d_y, y);

    delete a;
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

