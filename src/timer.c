#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "timer.h"


const double NANOSECOND_DIVISOR = 1000000000;


// NOTE: Super inaccurate implementation. Only for testing.
timeState* updateTimeState(timeState *state) {
	if (state == NULL) state = createTimeState();
	if (state == NULL) return NULL;

	struct timespec tv;
	if (clock_gettime(CLOCK_REALTIME, &tv) != 0) return 0;

	KTime currentTime = tv.tv_sec + (tv.tv_nsec / NANOSECOND_DIVISOR);
	(*state).deltaTime = currentTime - (*state).currentTime;
	(*state).currentTime = currentTime;

	return state;
}


timeState* createTimeState(void) {
	timeState *state = calloc(1, sizeof(timeState));
	if (state == NULL) return NULL;
	(*state).currentTime = (KTime) time(0);
	(*state).deltaTime = 0;
	return state;
}
