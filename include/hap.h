#ifndef __HAP_INCLUDED__
#define __HAP_INCLUDED__

#ifdef OS_Windows
#include <Windows.h>
#endif

typedef long double HAPTime;

typedef struct {
	HAPTime currentTime;
	HAPTime deltaTime;

	void *timespec;
} timeState;

typedef struct {
	timeState *time;
} HAPEngine;

void* hap_module_execute(HAPEngine *engine, char *identifier);

#endif