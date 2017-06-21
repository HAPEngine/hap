#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "timer.h"


timeState* updateTimeState(timeState *state) {
	if (state == NULL) state = createTimeState();
	if (state == NULL) return NULL;

	KTime currentTime = (KTime) clock();
	(*state).deltaTime = currentTime - (*state).currentTime;
	(*state).currentTime = currentTime;
	return state;
}


// NOTE: Super inaccurate implementation. Only for testing.
timeState* createTimeState(void) {
	timeState *state = malloc(sizeof(timeState));
	if (state == NULL) return NULL;
	(*state).currentTime = (KTime) clock();
	(*state).deltaTime = 0;
	return state;
}
