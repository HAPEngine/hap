#include <stdlib.h>

typedef struct {
	char *identifier;
} KModule;


KModule* kmodule_create(void) {
	KModule *module = malloc(sizeof(KModule));
	return module;
}


KModule* kmodule_load(char *identifier) {
	KModule *module = kmodule_create();
	(*module).identifier = identifier;
	return module;
}


KModule* kmodule_update(KModule *module) {}

KModule* kmodule_unload(KModule *module) {}

void kmodule_destroy(KModule *module) {
	free(module);
}
