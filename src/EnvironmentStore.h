
/* scan predator */
void environment_scan_init();
void environment_scan_destroy();
void environment_scan_onDraw(Display* d);
void environment_scan_onFrame(void *simulation);


void environment_food_init();
void environment_food_destroy();
void environment_food_round_start(void *simulation);
void environment_food_onFrame(void *simulation);

void environment_intersect_onFrame(void *simulation);

void measure_describe_init();
void measure_describe_round_start(void *simulation);
void measure_describe_round_end(void *simulation);
void measure_describe_destroy();
void measure_describe_get_vals(double *spread, double *speed);

//Note this uses the old -- one score for all method
/* FOOD */
/*void environment_food_init(int numberOfFood);
void environment_food_destroy();
void environment_food_onDraw(Display* d);
void environment_food_onFrame(void *simulation);
*/
