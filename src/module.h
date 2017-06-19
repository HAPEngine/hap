#include <kro.h>


KModule* kmodule_create(char *identifier);
void kmodule_load(KModule *module);
void kmodule_unload(KModule *module);
void kmodule_update(KModule *module);
void kmodule_destroy(KModule *module);
