#include <stdio.h>
#include <kro.h>


typedef struct KModule {
	void* (*create)(void);
	void (*load)(void *state, char *identifier);
	KTime (*update)(void* state);
	void (*unload)(void* state);
	void (*destroy)(void* state);

	KTime nextUpdate;

	char *identifier;
	void *ref;
	void *state;
} KModule;


/** Module lifecycle **/
KModule* kmodule_create(char *identifier);
void kmodule_load(KModule *module);
KTime kmodule_update(KModule *module);
void kmodule_unload(KModule *module);
void kmodule_destroy(KModule *module);
