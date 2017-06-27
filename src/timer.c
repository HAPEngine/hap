#ifndef _POSIX_C_SOURCE
/** Apparently we need to to tell time.h that it's 1993 :) **/
#define _POSIX_C_SOURCE 199309L
#endif

#include <stdio.h>

#include <stdlib.h>
#include <time.h>

#include "timer.h"


const double NANOSECOND_DIVISOR = 1000000000;


// NOTE: Super inaccurate implementation. Only for testing.
timeState* updateTimeState(timeState *state) {
	if (state == NULL) state = createTimeState();
	if (state == NULL) return NULL;

	struct timespec tv;
	if (clock_gettime(CLOCK_MONOTONIC, &tv) != 0) return 0;

	KTime currentTime = (KTime) tv.tv_sec + ((KTime) tv.tv_nsec / NANOSECOND_DIVISOR);
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
