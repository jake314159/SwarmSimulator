using namespace std;
#include <iostream>
#include <math.h>
// fastsin & fastcos implementation from here:
//      http://lab.polygonal.de/?p=205

#define LOOKUP_TABLE_SIZE 1000

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
double spacing; // 0.2
void setup_fast_math() {
    spacing = (max_atan-min_atan)/(double)LOOKUP_TABLE_SIZE;
    for(int i=0; i<LOOKUP_TABLE_SIZE; i++) {
        atan_lookup_table[i] = atan(min_atan+i*spacing);
    }
}
double fastatan(double x) {
    int index = 0;
    double t = ((x-min_atan));
    if(t>0) index = t/spacing;

    if(index<0) index = 0;
    if(index>(LOOKUP_TABLE_SIZE-1)) index = LOOKUP_TABLE_SIZE-1;
    return atan_lookup_table[index];
    //cout <<spacing <<"  "<< x << "\t\t" << index <<"\t\t"<<atan_lookup_table[index]<< endl;
    //return atan(x);
}

//https://en.wikipedia.org/wiki/Fast_inverse_square_root
double fastsqrt( double number )
{
	long i;
	double x2, y;
	const double threehalfs = 1.5F;
 
	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
 
	return y;
}

