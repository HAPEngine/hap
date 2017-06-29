#ifndef __HAP_INCLUDED__
#define __HAP_INCLUDED__


typedef long double KTime;

typedef struct {
	KTime currentTime;
	KTime deltaTime;

	void *timespec;
} timeState;

typedef struct {
	timeState *time;
} KSystem;

void* kmodule_execute(KSystem *system, char *identifier);


#endif
