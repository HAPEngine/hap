#ifndef __HAP_INCLUDED__
#define __HAP_INCLUDED__

#ifdef OS_Windows
#include <Windows.h>
#endif

#include <stdlib.h>

typedef long double HAPTime;

typedef struct {
	HAPTime currentTime;
	HAPTime timeDelta;

	void *timespec;
} timeState;

typedef struct {
	char *name;

	timeState *time;

	int  *argc;
	char **argvp;
} HAPEngine;

HAPEngine* hap_engine_create();
void* hap_module_execute(HAPEngine *engine, const short numModules, char *identifiers[]);
void hap_engine_destroy(HAPEngine* engine);

#endif
