
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

void environment_votex_onFrame(void *simulation);


void environment_confusion_onFrame(void *simulation);

/* Spread min/max */
//Should we minimise spread (true) or maximise (false)??
void environment_spread_setMinimise(bool mini);
void environment_spread_onFrame(void *simulation);

/* displacement min/max */
//Should we minimise center displacement (true) or maximise (false)??
void environment_center_disp_setMinimise(bool mini);
void environment_center_disp_onFrame(void *simulation);

/* NN-Distance min/max */
void environment_nnd_setMinimise(bool mini);
void environment_nnd_onFrame(void *simulation);

/* Polarity min/max */
void environment_polarity_setMinimise(bool mini);
void environment_polarity_onFrame(void *simulation);

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
