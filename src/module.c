#ifndef OS_Windows
#include <dlfcn.h>
#endif

#include <hap.h>
#include <stdio.h>
#include <stdlib.h>

#include "module.h"
#include "timer.h"


#define MODULES_PER_LIST_GROUP 16

/**
 * Avoid allowing simulating frames below 60FPS.
 *
 * If a module is slow to update, the we don't want to get into cases
 * where frames are given large amounts of time between eachother.
 *
 * For instance, an extreme example - it would be much more difficult to detect
 * all possible collisions that occured between objects in a 3D game within the
 * last 10 seconds so it makes more sense to simulate all modules one after
 * another in 16ms increments which provides less chance of missing collisions.
 *
 * Although less common, similar issues could surely occur with audio or other
 * types of systems as well.
 */
#define MINIMUM_FPS 60
#define MAX_SIMULATION_SLICE_TIME(fps) ((HAPTime) (1.0 / fps))

/* struct moduleList { */
/* 	HAPModule    **modules; */
/* 	moduleList *next; */
/* }; */

/* typedef struct moduleList moduleList; */

void* hap_module_execute(HAPEngine *engine, char *identifier) {
	timeState *time;
	HAPTime nextUpdate;
	HAPTime simulatedTime;
	HAPModule *m = hap_module_create(engine, identifier);

	if (m == NULL) return NULL;

	hap_module_load(engine, m);

	if ((*engine).time == NULL) (*engine).time = updateTimeState((*engine).time);

	if ((*engine).time == NULL) return NULL;

	time = (*engine).time;

	while ((*m).nextUpdate > -1) {
		updateTimeState((*engine).time);

		simulatedTime = 0;
		nextUpdate = (*m).nextUpdate;

		if ((*m).nextUpdate) {
			(*m).nextUpdate -= (*time).deltaTime;

			if ((*m).nextUpdate < 0) {
				(*m).nextUpdate = 0;
			}

		} else {
			while ((simulatedTime + (*m).nextUpdate) < (*time).deltaTime) {
				if (nextUpdate < 0) {
					(*m).nextUpdate = nextUpdate;
					break;
				}

				(*m).nextUpdate -= MAX_SIMULATION_SLICE_TIME(MINIMUM_FPS);

				if ((*m).nextUpdate < 0) (*m).nextUpdate = 0;

				nextUpdate = hap_module_update(engine, m);
				(*m).nextUpdate = nextUpdate;
				simulatedTime += MAX_SIMULATION_SLICE_TIME(MINIMUM_FPS);
			}
		}
	}

	hap_module_unload(engine, m);
	hap_module_destroy(engine, m);

	return (void*) m;
}

HAPModule* hap_module_create(HAPEngine *engine, char *identifier) {
	HAPModule *module = (HAPModule*) calloc(1, sizeof(HAPModule));

	if (module == NULL) return NULL;

	(*module).identifier = identifier;

#ifdef OS_Windows
	(*module).ref = (void*) LoadLibrary((*module).identifier);
#else
	(*module).ref = dlopen((*module).identifier, RTLD_NOW);
#endif

	if ((*module).ref == NULL) {
		printf("Failed to load module: %s\n", (*module).identifier);
		hap_module_destroy(engine, module);
		exit(EXIT_FAILURE);
		return NULL;
	}

#ifdef OS_Windows
	(*module).create = (void* (*)(HAPEngine* engine)) GetProcAddress((*module).ref, "create");
	(*module).load = (void(*)(HAPEngine* engine, void *state, char *identifier)) GetProcAddress((*module).ref, "load");
	(*module).update = (HAPTime(*)(HAPEngine* engine, void *state)) GetProcAddress((*module).ref, "update");
	(*module).unload = (void(*)(HAPEngine* engine, void *state)) GetProcAddress((*module).ref, "unload");
	(*module).destroy = (void(*)(HAPEngine* engine, void *state)) GetProcAddress((*module).ref, "destroy");
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
	(*module).create = (void* (*)(HAPEngine* engine)) dlsym((*module).ref, "create");
	(*module).load = (void (*)(HAPEngine* engine, void *state, char *identifier)) dlsym((*module).ref, "load");
	(*module).update = (HAPTime (*)(HAPEngine* engine, void *state)) dlsym((*module).ref, "update");
	(*module).unload = (void (*)(HAPEngine* engine, void *state)) dlsym((*module).ref, "unload");
	(*module).destroy = (void (*)(HAPEngine* engine, void *state)) dlsym((*module).ref, "destroy");
#pragma GCC diagnostic pop

	if ((*module).create)
		(*module).state = ((*module).create(engine));

#endif

	return module;
}

void hap_module_load(HAPEngine *engine, HAPModule *module) {
	if ((*module).load == NULL) return;

	(*module).load(engine, (*module).state, (*module).identifier);
}

HAPTime hap_module_update(HAPEngine *engine, HAPModule *module) {
	if ((*module).update == NULL) return 0;

	return (*module).update(engine, (*module).state);
}

void hap_module_unload(HAPEngine *engine, HAPModule *module) {
	if ((*module).unload == NULL) return;

	(*module).unload(engine, (*module).state);
}

void _module_close_ref(void* ref) {
	if (ref == NULL) return;

#ifdef OS_Windows
#else
	dlclose(ref);
#endif
}


void hap_module_destroy(HAPEngine *engine, HAPModule *module) {
	if ((*module).destroy != NULL)
		(*module).destroy(engine, (*module).state);

	_module_close_ref((*module).ref);

	(*module).state   = NULL;
	(*module).ref = NULL;

	free(module);
}
