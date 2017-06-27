#include <kro.h>
#include <stdio.h>
#include <stdlib.h>

#include "kro_example.h"


void* create(KSystem *system) {
	exampleState *state = (exampleState*) malloc(sizeof(exampleState));
	printf("[        ] Creating example module\n");
	return (void*) state;
}


void load(KSystem *system, exampleState *state, char *identifier) {
	(*state).name = identifier;
	printf("[        ] Loading %s\n", (*state).name);
}


KTime update(KSystem *system, exampleState *state) {
	printf("[%lf] Updating example module at %s\n", (*(*system).time).deltaTime, (*state).name);

	// Run this function no more often than once every quarter second
	return 0.25;
}


void unload(KSystem *system, exampleState *state) {
	printf("[%lf] Unloading example module at %s\n", (*(*system).time).deltaTime, (*state).name);
}


void destroy(KSystem *system, exampleState *state) {
	printf("[%lf] Destroying example module at %s\n", (*(*system).time).deltaTime, (*state).name);
}
