using namespace std;
#include <iostream>
#include <math.h>
// fastsin & fastcos implementation from here:
//      http://lab.polygonal.de/?p=205

#define LOOKUP_TABLE_SIZE 1000
#define LOOKUP_TABLE_SIZE_SQRT 10000

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

double atan_lookup_table[LOOKUP_TABLE_SIZE];
double min_atan = -10.0;
double max_atan = 10.0;
double atan_spacing; // 0.2

void setup_fast_math() {
    atan_spacing = (max_atan-min_atan)/(double)LOOKUP_TABLE_SIZE;
    for(int i=0; i<LOOKUP_TABLE_SIZE; i++) {
        atan_lookup_table[i] = atan(min_atan+i*atan_spacing);
    }
}
double fastatan(double x) {
    int index = 0;
    double t = ((x-min_atan));
    if(t>0) index = t/atan_spacing;

    if(index<0) index = 0;
    if(index>(LOOKUP_TABLE_SIZE-1)) index = LOOKUP_TABLE_SIZE-1;
    return atan_lookup_table[index];
    //cout <<spacing <<"  "<< x << "\t\t" << index <<"\t\t"<<atan_lookup_table[index]<< endl;
    //return atan(x);
}


//http://www.phailed.me/2014/10/0x5f400000-understanding-fast-inverse-sqrt-the-easyish-way/
double fastsqrt( double x )
{

    return sqrt(x);
}

