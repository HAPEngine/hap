#ifndef OS_Windows
#include <dlfcn.h>
#endif

#include <hap.h>
#include <math.h>
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
#define GET_MAX_SIMULATION_FRAME_TIME(fps) ((HAPTime) (1.0 / fps))

#ifndef MAX_SIMULATION_FRAME_TIME
#define MAX_SIMULATION_FRAME_TIME GET_MAX_SIMULATION_FRAME_TIME(MINIMUM_FPS)
#endif

typedef struct moduleList moduleList;

struct moduleList {
	HAPModule    *module;

	moduleList *next;
};

HAPModule* _hap_module_update_loop(HAPEngine *engine, short numModules, HAPModule **modules) {
	short index;

	HAPTime simulatedTime;
	HAPTime simulatedTimeDelta;

	HAPTime actualTime;
	HAPTime actualTimeDelta;

	for (;;) {
		simulatedTime = (*(*engine).time).currentTime;
		simulatedTimeDelta = (*(*engine).time).timeDelta;

		hap_timer_update((*engine).time);

		actualTime = (*(*engine).time).currentTime;
		actualTimeDelta = (*(*engine).time).timeDelta;

		while (simulatedTime < actualTime) {
			if (simulatedTimeDelta > MAX_SIMULATION_FRAME_TIME)
				simulatedTimeDelta = MAX_SIMULATION_FRAME_TIME;

			(*(*engine).time).currentTime = simulatedTime;
			(*(*engine).time).timeDelta = simulatedTimeDelta;

			for (index = 0; index < numModules; ++index) {
				if ((*modules[index]).nextUpdate > simulatedTime) continue;

				(*modules[index]).nextUpdate = hap_module_update(engine, modules[index]);

				if ((*modules[index]).nextUpdate < 0) return modules[index];

				(*modules[index]).nextUpdate += simulatedTime;
			}

			(*(*engine).time).currentTime = actualTime;

			simulatedTime += simulatedTimeDelta;

			simulatedTimeDelta = fmod((actualTime - simulatedTime), MAX_SIMULATION_FRAME_TIME);
		}
	}

	return NULL;
}

void* hap_module_execute(HAPEngine *engine, const short numModules, char *identifiers[]) {
	HAPTime nextUpdate;
	short index;
	timeState *time;

	HAPModule **modules = (HAPModule**) calloc(numModules, sizeof(HAPModule*));

	for (index = 0; index < numModules; ++index) {
		modules[index] = hap_module_create(engine, identifiers[index]);

		// Creating a module failed, so destroy previously created ones
		if (modules[index] == NULL) {
			--index;

			for (; index >= 0; --index) {
				hap_module_destroy(engine, modules[index]);
				modules[index] = NULL;
			}

			return NULL;
		}
	}

	for (index = 0; index < numModules; ++index) {
		hap_module_load(engine, modules[index]);
	}


	time = (*engine).time;

	_hap_module_update_loop(engine, numModules, modules);

	for (index = 0; index < numModules; ++index) hap_module_unload(engine, modules[index]);

	for (index = 0; index < numModules; ++index) hap_module_destroy(engine, modules[index]);

	return (void*) modules;
}

HAPModule* hap_module_create(HAPEngine *engine, char *identifier) {
	HAPModule *module = (HAPModule*) calloc(1, sizeof(HAPModule));

	if (module == NULL) return NULL;

	(*module).identifier = identifier;
	(*module).nextUpdate = 0;

#ifdef OS_Windows
	(*module).ref = (void*) LoadLibrary((*module).identifier);
#else
	(*module).ref = dlopen((*module).identifier, RTLD_NOW);
#endif

	if ((*module).ref == NULL) {
		fprintf(stderr, "Failed to load module: %s\n", (*module).identifier);
		fprintf(stderr, "Error was: %s\n\n", dlerror());

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

#ifndef OS_Windows
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
