using namespace std;
#include <iostream>
#include "Vector2d.h"

int main() {
    cout << "Looks like everything is good" << endl;

    double val_x = 2.0;
    double val_y = 4.0;
    double val_x_2 = 2.0;
    double val_y_2 = 4.0;
    Vector2d *l = new Vector2d( val_x, val_y );
    Vector2d *r = new Vector2d( val_x_2, val_y_2 );
    cout << (*l==*r) << endl;
    delete l;
    delete r;
}
