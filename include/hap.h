#ifndef __HAP_INCLUDED__
#define __HAP_INCLUDED__

#ifdef OS_Windows
#include <Windows.h>
#endif

#include <stdlib.h>

typedef long double HAPTime;

enum HAPLogLevel {
	LOGLEVEL_INFO = 0,
	LOGLEVEL_NOTICE = 10,
	LOGLEVEL_WARNING = 20,
	LOGLEVEL_ERROR = 30,
	LOGLEVEL_FATAL = 99,
};

typedef enum HAPLogLevel HAPLogLevel;


struct timeState {
	HAPTime currentTime;
	HAPTime timeDelta;

	void *timespec;
};

typedef struct timeState timeState;


struct HAPEngine {
	HAPLogLevel loglevel;
	char *name;

	timeState *time;

	int  *argc;
	char **argvp;
};

typedef struct HAPEngine HAPEngine;


HAPEngine* hap_engine_create(char *name);
void* hap_module_execute(HAPEngine *engine, const short numModules, char *identifiers[]);
void hap_engine_destroy(HAPEngine* engine);
void hap_log(HAPEngine *engine, HAPLogLevel level, char *message);
#endif
