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
	exampleState *state = (exampleState*) malloc(sizeof(exampleState));
	return (void*) state;
}


void load(HAPEngine *engine, exampleState *state, char *identifier) {
	(*state).name = identifier;
}


HAPTime update(HAPEngine *engine, exampleState *state) {
	// Update this module no more often than once every quarter second
	return (HAPTime) 0.25;
}


void unload(HAPEngine *engine, exampleState *state) {
	printf("Unloading %s\n", (*state).name);
}


void destroy(HAPEngine *engine, exampleState *state) {}


#ifndef OS_Windows
#pragma GCC diagnostic pop
#endif