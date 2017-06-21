#include <kro.h>
#include <stdio.h>
#include <stdlib.h>

#include "kro_example.h"


void* create(void) {
	exampleState *state = (exampleState*) malloc(sizeof(exampleState));
	printf("Creating example module.\n");
	return (void*) state;
}


void load(exampleState *state, char *identifier) {
	(*state).name = identifier;
	printf("Loading example from %s\n", (*state).name);
}


KTime update(exampleState *state) {
	printf("Updating example module at %s\n", (*state).name);
	return 0;
}


void unload(exampleState *state) {
	printf("Unloading example module at %s\n", (*state).name);
}


void destroy(exampleState *state) {
	printf("Destroying example module at %s\n", (*state).name);
}
