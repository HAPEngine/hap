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
		(*state).currentTime = (HAPTime) time(NULL);
		(*state).deltaTime = 0;
		(*state).timespec = calloc(1, sizeof(timespec));
	}

	if (state == NULL) return NULL;
	timespec *tv = (timespec*) (*state).timespec;
	if (tv == NULL) return NULL;

	if (clock_gettime(CLOCK_MONOTONIC, tv) != 0) return 0;

	HAPTime currentTime = (HAPTime) (*tv).tv_sec + ((HAPTime) (*tv).tv_nsec / (HAPTime) 1e9);
	(*state).deltaTime = currentTime - (*state).currentTime;
	(*state).currentTime = currentTime;

	return state;
}


void destroyTimeState(timeState *state) {
	(*state).currentTime = 0;
	(*state).deltaTime = 0;

	if ((*state).timespec != NULL) {
		free((*state).timespec);
		(*state).timespec = NULL;
	}

	free(state);
}
