#include <hap.h>


typedef struct {
	char *name;
} exampleState;


extern void* create(HAPEngine *engine);
extern void load(HAPEngine *engine, exampleState *state, char *identifier);
extern HAPTime update(HAPEngine *engine, exampleState *state);
extern void unload(HAPEngine *engine, exampleState *state);
extern void destroy(HAPEngine *engine, exampleState *state);
