#include "../src/Vector2d.cpp"
#include <gtest/gtest.h>
 
TEST(Vector2d_test, set_X_positive) { 
    Vector2d *v = new Vector2d;
    double value = 1.1;
    v->setX(value);
    ASSERT_EQ(value, v->getX());
    delete v;
}

TEST(Vector2d_test, set_X_negitive) { 
    Vector2d *v = new Vector2d;
    double value = -1.1;
    v->setX(value);
    ASSERT_EQ(value, v->getX());
    delete v;
}
 
TEST(Vector2d_test, set_Y_positive) { 
    Vector2d *v = new Vector2d;
    double value = 1.1;
    v->setY(value);
    ASSERT_EQ(value, v->getY());
    delete v;
}

TEST(Vector2d_test, set_Y_negitive) { 
    Vector2d *v = new Vector2d;
    double value = -1.1;
    v->setY(value);
    ASSERT_EQ(value, v->getY());
    delete v;
}

TEST(Vector2d_test, constructor) { 
    double val_x = 123.4;
    double val_y = 567.8;
    Vector2d *v = new Vector2d( val_x, val_y );
    ASSERT_EQ(val_x, v->getX());
    ASSERT_EQ(val_y, v->getY());
    delete v;
}

TEST(Vector2d_test, operator_plus_eq) { 
    double val_x = 2.0;
    double val_y = 4.0;
    double val_x_2 = 1.0;
    double val_y_2 = 1.0;
    Vector2d *l = new Vector2d( val_x, val_y );
    Vector2d *r = new Vector2d( val_x_2, val_y_2 );
    *l += *r;
    ASSERT_EQ(l->getX(), val_x+val_x_2);
    ASSERT_EQ(l->getY(), val_y+val_y_2);
    delete l;
    delete r;
}

TEST(Vector2d_test, operator_div_eq_double) { 
    double val_x = 2.0;
    double val_y = 4.0;
    Vector2d *l = new Vector2d( val_x, val_y );
    *l /= 2;
    ASSERT_EQ(l->getX(), val_x/2);
    ASSERT_EQ(l->getY(), val_y/2);
    delete l;
}

TEST(Vector2d_test, operator_comparison_equal) { 
    double val_x = 2.0;
    double val_y = 4.0;
    double val_x_2 = 2.0;
    double val_y_2 = 4.0;
    Vector2d *l = new Vector2d( val_x, val_y );
    Vector2d *r = new Vector2d( val_x_2, val_y_2 );
    ASSERT_TRUE(*l==*l);
    ASSERT_TRUE(*r==*r);
    ASSERT_TRUE(*l==*r);
    ASSERT_TRUE(*r==*l);
    delete l;
    delete r;
}

TEST(Vector2d_test, operator_comparison_not_equal_X) { 
    double val_x = 2.0;
    double val_y = 4.0;
    double val_x_2 = 3.0;
    double val_y_2 = 4.0;
    Vector2d *l = new Vector2d( val_x, val_y );
    Vector2d *r = new Vector2d( val_x_2, val_y_2 );
    ASSERT_TRUE(*l!=*r);
    ASSERT_TRUE(*r!=*l);
    delete l;
    delete r;
}

TEST(Vector2d_test, operator_comparison_not_equal_Y) { 
    double val_x = 2.0;
    double val_y = 4.0;
    double val_x_2 = 3.0;
    double val_y_2 = 5.0;
    Vector2d *l = new Vector2d( val_x, val_y );
    Vector2d *r = new Vector2d( val_x_2, val_y_2 );
    ASSERT_TRUE(*l!=*r);
    ASSERT_TRUE(*r!=*l);
    delete l;
    delete r;
}

TEST(Vector2d_test, set_vector_from_vector) { 
    double val_x = 2.0;
    double val_y = 4.0;
    double val_x_2 = 3.0;
    double val_y_2 = 5.0;
    Vector2d *l = new Vector2d( val_x, val_y );
    Vector2d *r = new Vector2d( val_x_2, val_y_2 );
    l->setVector(r);
    ASSERT_EQ(l->getX(), val_x_2);
    ASSERT_EQ(l->getY(), val_y_2);
    delete l;
    delete r;
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
