#include <hap.h>


typedef struct {
	char *name;
} exampleState;


extern void* create(KSystem *system);
extern void load(KSystem *system, exampleState *state, char *identifier);
extern KTime update(KSystem *system, exampleState *state);
extern void unload(KSystem *system, exampleState *state);
extern void destroy(KSystem *system, exampleState *state);
