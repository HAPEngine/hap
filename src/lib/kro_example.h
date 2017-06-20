typedef struct {
	char *name;
} exampleState;


extern void* create(void);
extern void load(exampleState *state, char *identifier);
extern void update(exampleState *state);
extern void unload(exampleState *state);
extern void destroy(exampleState *state);
