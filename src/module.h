#include <stdio.h>
#include <kro.h>


typedef struct KModule {
	char *identifier;
	void *ref;
	void *state;

	void* (*create)(void);
	void (*load)(void *state, char *identifier);
	void (*update)(void* state);
	void (*unload)(void* state);
	void (*destroy)(void* state);
} KModule;


KModule* kmodule_create(char *identifier);
void kmodule_load(KModule *module);
void kmodule_unload(KModule *module);
void kmodule_update(KModule *module);
void kmodule_destroy(KModule *module);
