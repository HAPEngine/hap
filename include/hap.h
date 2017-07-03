#ifndef __HAP_INCLUDED__
#define __HAP_INCLUDED__

#ifdef OS_Windows
#include <Windows.h>
#endif

#include <stdlib.h>

typedef long double HAPTime;

typedef struct {
	HAPTime currentTime;
	HAPTime deltaTime;

	void *timespec;
} timeState;

typedef struct {
	char *name;

	timeState *time;

	int  *argc;
	char **argvp;
} HAPEngine;

void* hap_module_execute(HAPEngine *engine, char *identifier);

#endif
