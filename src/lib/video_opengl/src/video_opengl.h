#include <hap.h>

void* create(HAPEngine *engine);
void load(HAPEngine *engine, void *state, char *identifier);
HAPTime update(HAPEngine *engine, void *state);
void unload(HAPEngine *engine, void *state);
void destroy(HAPEngine *engine, void *state);
