using namespace std;
#include <iostream>
#include <math.h>
// fastsin & fastcos implementation from here:
//      http://lab.polygonal.de/?p=205

#define LOOKUP_TABLE_SIZE 1000
#define LOOKUP_TABLE_SIZE_SQRT 10000

double fastsin(double x) {
    #ifdef DEBUG
        return sin(x);
    #else
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
    #endif
}

double fastcos(double x) {
    #ifdef DEBUG
        return cos(x);
    #else
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
    #endif
}

//https://stackoverflow.com/questions/3380628/fast-arc-cos-algorithm
double fastacos(double x) {
    #ifdef DEBUG
        return acos(x);
    #else
        return (-0.69813170079773212 * x * x - 0.87266462599716477) * x + 1.5707963267948966;
    #endif
}

double atan_lookup_table[LOOKUP_TABLE_SIZE];
double min_atan = -10.0;
double max_atan = 10.0;
double atan_spacing; // 0.2

void setup_fast_math() {
    #ifdef DEBUG

    #else
        atan_spacing = (max_atan-min_atan)/(double)LOOKUP_TABLE_SIZE;
        for(int i=0; i<LOOKUP_TABLE_SIZE; i++) {
            atan_lookup_table[i] = atan(min_atan+i*atan_spacing);
        }
    #endif
}
double fastatan(double x) {
    #ifdef DEBUG
        return atan(x);
    #else
        int index = 0;
        double t = ((x-min_atan));
        if(t>0) index = t/atan_spacing;

        if(index<0) index = 0;
        if(index>(LOOKUP_TABLE_SIZE-1)) index = LOOKUP_TABLE_SIZE-1;
        return atan_lookup_table[index];
    #endif
}


//http://www.phailed.me/2014/10/0x5f400000-understanding-fast-inverse-sqrt-the-easyish-way/
double fastsqrt( double x )
{
    return sqrt(x);
}

