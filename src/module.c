#include <dlfcn.h>
#include <kro.h>
#include <stdio.h>
#include <stdlib.h>

#include "module.h"
#include "timer.h"

#define SIMULATION_SLICE_TIME 1 / 60

void* kmodule_execute(char *identifier, timeState* state) {
	KTime simulatedTime;
	KModule *m = kmodule_create(identifier);
	if (m == NULL) return NULL;

	kmodule_load(m);

	if (state == NULL) state = updateTimeState(state);
	if (state == NULL) return NULL;

	// TODO: Handle keyboard interrupt?
	while ((*m).nextUpdate > -1) {
		updateTimeState(state);
		simulatedTime = 0;

		if ((*m).nextUpdate) {
			(*m).nextUpdate -= (*state).deltaTime;
			if ((*m).nextUpdate < 0) (*m).nextUpdate = 0;
		} else {
			while ((simulatedTime + (*m).nextUpdate) < (*state).deltaTime) {
				(*m).nextUpdate -= SIMULATION_SLICE_TIME;
				if ((*m).nextUpdate < 0) (*m).nextUpdate = 0;
				(*m).nextUpdate += kmodule_update(m);
			}
		}
	}

	kmodule_unload(m);
	kmodule_destroy(m);
	return (void*) m;
}


KModule* kmodule_create(char *identifier) {
	KModule *module = (KModule*) calloc(1, sizeof(KModule));
	if (module == NULL) return NULL;

	(*module).identifier = identifier;
	(*module).ref = dlopen((*module).identifier, RTLD_NOW);

	if ((*module).ref == NULL) {
		printf("Failed to load module: %s\nError was: %s\n", (*module).identifier, dlerror());
		free(module);
		exit(EXIT_FAILURE);
		return NULL;
	}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
	(*module).create = (void* (*)(void)) dlsym((*module).ref, "create");
	(*module).load = (void (*)(void *state, char *identifier)) dlsym((*module).ref, "load");
	(*module).update = (KTime (*)(void* state)) dlsym((*module).ref, "update");
	(*module).unload = (void (*)(void* state)) dlsym((*module).ref, "unload");
	(*module).destroy = (void (*)(void* state)) dlsym((*module).ref, "destroy");
#pragma GCC diagnostic pop

	if ((*module).create)
		(*module).state = ((*module).create());

	return module;
}


void kmodule_load(KModule *module) {
	if ((*module).load == NULL) return;
	(*module).load((*module).state, (*module).identifier);
}


KTime kmodule_update(KModule *module) {
	if ((*module).update == NULL) return 0;
	return (*module).update((*module).state);
}


void kmodule_unload(KModule *module) {
	if ((*module).unload == NULL) return;
	(*module).unload((*module).state);
}


void kmodule_destroy(KModule *module) {
	if ((*module).destroy != NULL)
		(*module).destroy((*module).state);

	(*module).state = NULL;
	dlclose((*module).ref);
	free(module);
}
