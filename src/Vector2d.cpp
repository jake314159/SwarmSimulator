using namespace std;
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "Vector2d.h"
#include "fastMath.h"

Vector2d::Vector2d () {

}

Vector2d::Vector2d (double x, double y) {
    this->x = x;
    this->y = y;
}

double Vector2d::getX() {
    return this->x;
}

void Vector2d::setX(double x) {
    this->x = x;
}

double Vector2d::getY() {
    return this->y;
}

void Vector2d::setY(double y) {
    this->y = y;
}

void Vector2d::setVector(double x, double y) {
    this->x = x;
    this->y = y;
}

/* Operators */
Vector2d& Vector2d::operator += (Vector2d& right)
{
    this->x += right.getX();
    this->y += right.getY();
    return *this;
}

bool Vector2d::operator == (Vector2d& right)
{
    //cout << "this.x" << this->x << "right.x" << right.getX() << endl;
    return (this->x == right.getX()) && (this->y == right.getY());
}

bool Vector2d::operator != (Vector2d& right) {
    return (this->x != right.getX()) || (this->y != right.getY());
}

Vector2d& Vector2d::operator /= (double right)
{
    this->x /= right;
    this->y /= right;
    return *this;
}

Vector2d& Vector2d::operator *= (double right)
{
    this->x *= right;
    this->y *= right;
    return *this;
}

void Vector2d::setVector(Vector2d *v)
{
    this->x = v->getX();
    this->y = v->getY();
}
//TODO test
double Vector2d::getMagnitude() {
    double a = this->x;
    double b = this->y;

    if((a + b) == 0.0) {
        return 0.0;
    } else {
        return sqrt(a*a + b*b);
    }
}

void Vector2d::randomize() {
    double rand_val = (((double)rand()/(double)RAND_MAX)*2.0) -1.;

    //Set Y so magnitude = 1
    double y_val = sqrt(1.0 - rand_val*rand_val);
    if( (rand()&1) == 0) {
        y_val = -y_val;
    }
    this->setX(rand_val);
    this->setY(y_val);
}
