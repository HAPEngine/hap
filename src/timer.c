#ifndef _POSIX_C_SOURCE
/** Apparently we need to to tell time.h that it's 1993 :) **/
#define _POSIX_C_SOURCE 199309L
#endif

#ifdef OS_Windows
#include <sys/timeb.h>
#else
#include <time.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "timer.h"


typedef struct timespec timespec;


timeState* hap_timer_update(timeState *state) {
#ifdef OS_Windows
	HAPTime currentTime = (HAPTime) (GetTickCount64() / 1000);

	if (state == NULL) {
		state = (timeState*) calloc(1, sizeof(timeState));

		if (state == NULL) return NULL;

		(*state).timespec = NULL;
		(*state).currentTime = 0;
		(*state).timeDelta = 0;

		hap_timer_update(state);
	}

#else

	if (state == NULL) {
		state = (timeState*) calloc(1, sizeof(timeState));

		if (state == NULL) return NULL;

		(*state).currentTime = 0;
		(*state).timeDelta = 0;
		(*state).timespec = calloc(1, sizeof(timespec));

		hap_timer_update(state);
	}

	timespec *tv = (timespec*) (*state).timespec;

	if (tv == NULL) return NULL;

	if (clock_gettime(CLOCK_MONOTONIC, tv) != 0) return 0;

	HAPTime currentTime = (HAPTime) (*tv).tv_sec + ((HAPTime) (*tv).tv_nsec / (HAPTime) 1e9);
#endif

	(*state).timeDelta = currentTime - (*state).currentTime;
	(*state).currentTime = currentTime;

	return state;
}

void hap_timer_destroy(timeState *state) {
	(*state).currentTime = 0;
	(*state).timeDelta = 0;

	if ((*state).timespec != NULL) {
		free((*state).timespec);
		(*state).timespec = NULL;
	}

	free(state);
}
