#include <dlfcn.h>
#include <kro.h>
#include <stdio.h>
#include <stdlib.h>

#include "module.h"
#include "timer.h"


// Assume a max FPS of 248
#define SIMULATION_SLICE_TIME (1.0 / 248)


void* kmodule_execute(KSystem *system, char *identifier) {
	timeState *time;
	KTime nextUpdate;
	KTime simulatedTime;
	KModule *m = kmodule_create(system, identifier);
	if (m == NULL) return NULL;

	kmodule_load(system, m);

	if ((*system).time == NULL) (*system).time = updateTimeState((*system).time);
	if ((*system).time == NULL) return NULL;

	time = (*system).time;

	// TODO: Handle keyboard interrupt?
	while ((*m).nextUpdate > -1) {
		updateTimeState((*system).time);

		simulatedTime = 0;
		nextUpdate = (*m).nextUpdate;

		if ((*m).nextUpdate) {
			(*m).nextUpdate -= (*time).deltaTime;

			if ((*m).nextUpdate < 0) {
				(*m).nextUpdate = 0;
			}
		} else {
			while ((simulatedTime + (*m).nextUpdate) < (*time).deltaTime) {
				if (nextUpdate < 0) break;
				(*m).nextUpdate -= SIMULATION_SLICE_TIME;
				if ((*m).nextUpdate < 0) (*m).nextUpdate = 0;
				nextUpdate = kmodule_update(system, m);
				(*m).nextUpdate = nextUpdate;
				simulatedTime += SIMULATION_SLICE_TIME;
			}
		}
	}

	kmodule_unload(system, m);
	kmodule_destroy(system, m);
	return (void*) m;
}


KModule* kmodule_create(KSystem *system, char *identifier) {
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
	(*module).create = (void* (*)(KSystem* system)) dlsym((*module).ref, "create");
	(*module).load = (void (*)(KSystem* system, void *state, char *identifier)) dlsym((*module).ref, "load");
	(*module).update = (KTime (*)(KSystem* system, void *state)) dlsym((*module).ref, "update");
	(*module).unload = (void (*)(KSystem* system, void *state)) dlsym((*module).ref, "unload");
	(*module).destroy = (void (*)(KSystem* system, void *state)) dlsym((*module).ref, "destroy");
#pragma GCC diagnostic pop

	if ((*module).create)
		(*module).state = ((*module).create(system));

	return module;
}


void kmodule_load(KSystem *system, KModule *module) {
	if ((*module).load == NULL) return;
	(*module).load(system, (*module).state, (*module).identifier);
}


KTime kmodule_update(KSystem *system, KModule *module) {
	if ((*module).update == NULL) return 0;
	return (*module).update(system, (*module).state);
}


void kmodule_unload(KSystem *system, KModule *module) {
	if ((*module).unload == NULL) return;
	(*module).unload(system, (*module).state);
}


void kmodule_destroy(KSystem *system, KModule *module) {
	if ((*module).destroy != NULL)
		(*module).destroy(system, (*module).state);

	(*module).state = NULL;
	dlclose((*module).ref);
	free(module);
}
