/**
 * This is an example module for HAP.
 *
 * It's worth mention that there are some #pragma statements in here, but they
 * are hopefully not necessary in production code since production code will
 * actually use the engine.
 *
 */


#ifndef OS_Windows
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif


#include <hap.h>
#include <stdio.h>
#include <stdlib.h>

#include "hap_example.h"


void* create(HAPEngine *engine) {
	(void)engine;      // Mark variable as used to avoid compiler warnings

	return (void*) malloc(sizeof(exampleState));
}


void load(HAPEngine *engine, exampleState *state, char *identifier) {
	(void)engine;      // Mark variable as used to avoid compiler warnings
	(void)identifier;  // Mark variable as used to avoid compiler warnings

	(*state).name = identifier;
}


HAPTime update(HAPEngine *engine, exampleState *state) {
	(void)engine;      // Mark variable as used to avoid compiler warnings
	(void)state;       // Mark variable as used to avoid compiler warnings

	// Update this module no more often than once every quarter second
	return (HAPTime) 0.25;
}


void unload(HAPEngine *engine, exampleState *state) {
	(void)engine;      // Mark variable as used to avoid compiler warnings
	(void)state;       // Mark variable as used to avoid compiler warnings
}


void destroy(HAPEngine *engine, exampleState *state) {
	(void)engine;      // Mark variable as used to avoid compiler warnings
	(void)state;       // Mark variable as used to avoid compiler warnings
}


#ifndef OS_Windows
#pragma GCC diagnostic pop
#endif
