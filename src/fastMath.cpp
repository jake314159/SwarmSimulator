using namespace std;
#include <iostream>
#include <math.h>
// fastsin & fastcos implementation from here:
//      http://lab.polygonal.de/?p=205

double fastsin(double x) {
    double sin;
    //always wrap input angle to -PI..PI
    while (x < -3.14159265)
        x += 6.28318531;
    while (x >  3.14159265)
        x -= 6.28318531;

    //compute sine
    if (x < 0)
        sin = 1.27323954 * x + .405284735 * x * x;
    else
        sin = 1.27323954 * x - 0.405284735 * x * x;
    return sin;
}

double fastcos(double x) {
    double cos;
    //always wrap input angle to -PI..PI
    while (x < -3.14159265)
        x += 6.28318531;
    while (x >  3.14159265)
        x -= 6.28318531;

    //compute cosine: sin(x + PI/2) = cos(x)
    x += 1.57079632;
    if (x >  3.14159265)
        x -= 6.28318531;

    if (x < 0)
        cos = 1.27323954 * x + 0.405284735 * x * x;
    else
        cos = 1.27323954 * x - 0.405284735 * x * x;
    return cos;
}

//https://stackoverflow.com/questions/3380628/fast-arc-cos-algorithm
double fastacos(double x) {
   return (-0.69813170079773212 * x * x - 0.87266462599716477) * x + 1.5707963267948966;
}

