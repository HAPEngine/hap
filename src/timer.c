#ifndef _POSIX_C_SOURCE
/** Apparently we need to to tell time.h that it's 1993 :) **/
#define _POSIX_C_SOURCE 199309L
#endif

#include <stdio.h>

#include <stdlib.h>
#include <time.h>

#include "timer.h"


typedef struct timespec timespec;


timeState* updateTimeState(timeState *state) {
	if (state == NULL) {
		state = (timeState*) calloc(1, sizeof(timeState));
		(*state).currentTime = (KTime) time(NULL);
		(*state).deltaTime = 0;
	}

	if (state == NULL) return NULL;

	timespec tv;
	if (clock_gettime(CLOCK_MONOTONIC, &tv) != 0) return 0;

	KTime currentTime = (KTime) tv.tv_sec + ((KTime) tv.tv_nsec / 1e9);
	(*state).deltaTime = currentTime - (*state).currentTime;
	(*state).currentTime = currentTime;

	return state;
}
