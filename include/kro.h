#ifndef __KRO_INCLUDED__
#define __KRO_INCLUDED__


typedef long double KTime;

typedef struct {
	KTime currentTime;
	KTime deltaTime;
} timeState;

typedef struct {
	timeState *time;
} KSystem;

void* kmodule_execute(KSystem *system, char *identifier);


#endif
