#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <kro.h>

#include "module.h"

void* kmodule_execute(char *identifier) {
	KModule *m = kmodule_create(identifier);
	if (m == NULL) return NULL;

	kmodule_load(m);
	kmodule_update(m);
	kmodule_unload(m);
	kmodule_destroy(m);

	return (void *)m;
}

KModule* kmodule_create(char *identifier) {
	KModule *module = (KModule*) malloc(sizeof(KModule));
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
	(*module).load = (void (*)(char *identifier)) dlsym((*module).ref, "load");
	(*module).update = (void (*)(void* state)) dlsym((*module).ref, "update");
	(*module).unload = (void (*)(void* state)) dlsym((*module).ref, "unload");
	(*module).destroy = (void (*)(void* state)) dlsym((*module).ref, "destroy");
#pragma GCC diagnostic pop

	if ((*module).create)
		(*module).state = ((*module).create());

	return module;
}

void kmodule_load(KModule *module) {
	if ((*module).load != NULL)
		(*module).load((*module).identifier);
}

void kmodule_update(KModule *module) {
	if ((*module).update != NULL)
		(*module).update((*module).state);
}

void kmodule_unload(KModule *module) {
	if ((*module).unload != NULL)
		(*module).unload((*module).state);
}

void kmodule_destroy(KModule *module) {
	if ((*module).destroy != NULL)
		(*module).destroy((*module).state);

	(*module).state = NULL;
	dlclose((*module).ref);
	free(module);
}
