/**
 * OpenGL module driver for HAP.
 */

#include <GL/freeglut.h>

#include <hap.h>

void* create(HAPEngine *engine) { return NULL; }
void load(HAPEngine *engine, void *state, char *identifier) {}
HAPTime update(HAPEngine *engine, void *state) { return 1; }
void unload(HAPEngine *engine, void *state) {}
void destroy(HAPEngine *engine, void *state) {}