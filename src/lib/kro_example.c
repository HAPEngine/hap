#include <stdlib.h>
#include <stdio.h>

#include "kro_example.h"

typedef struct {
	void *example;
} exampleState;

void* create(void) {
	exampleState *state = (exampleState*) malloc(sizeof(exampleState));
	printf("Creating example module.\n");
	return (void*) state;
}

void load(void *state, char *identifier) {
	printf("Loading example from: %s\n", identifier);
}

void update(void* state) {
	printf("Updating example module.\n");
}

void unload(void* state) {
	printf("Unloading example module.\n");
}

void destroy(void* state) {
	printf("Destroying example module.\n");
}
