#include <stdio.h>
#include <hap.h>


typedef struct KModule {
	void* (*create)(KSystem *system);
	void (*load)(KSystem *system, void *state, char *identifier);
	KTime (*update)(KSystem *system, void* state);
	void (*unload)(KSystem *system, void* state);
	void (*destroy)(KSystem *system, void* state);

	KTime nextUpdate;

	char *identifier;
	void *ref;
	void *state;
} KModule;


/** Module lifecycle **/
KModule* kmodule_create(KSystem *system, char *identifier);
void kmodule_load(KSystem *system, KModule *module);
KTime kmodule_update(KSystem *system, KModule *module);
void kmodule_unload(KSystem *system, KModule *module);
void kmodule_destroy(KSystem *system, KModule *module);
