
#include "Environment.h"

Environment::Environment() {
	onDraw = NULL;
	onFrame = NULL;
	roundStart = NULL;
	roundEnd = NULL;
	init = NULL;
	destroy = NULL;
}