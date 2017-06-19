#include <dlfcn.h>
#include <stdlib.h>

#include <kro.h>

#include "module.h"


KModule* kmodule_create(char *identifier) {
	KModule *module = (KModule*) malloc(sizeof(KModule));
	(*module).identifier = identifier;
	(*module).ref = dlopen((*module).identifier, RTLD_NOW);
	/* (*module).state = ((*module).create()); */
	return module;
}


void kmodule_load(KModule *module) {
	/* (*module).load((*module).identifier); */
}

void kmodule_update(KModule *module) {
	/* (*module).update((*module).state); */
}

void kmodule_unload(KModule *module) {
	/* (*module).unload((*module).state); */
}

void kmodule_destroy(KModule *module) {
	/* (*module).destroy((*module).state); */
	(*module).state = NULL;
	dlclose((*module).ref);
	free(module);
}

KModule* execute_module(char *identifier) {
	KModule *m = kmodule_create(identifier);
	kmodule_load(m);
	kmodule_update(m);
	kmodule_unload(m);
	kmodule_destroy(m);
	return m;
}
