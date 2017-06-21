#ifndef __KRO_INCLUDED__
#define __KRO_INCLUDED__


typedef double KTime;

typedef struct {
	KTime currentTime;
	KTime deltaTime;
} timeState;

void* kmodule_execute(char *identifier, timeState* state);


#endif
