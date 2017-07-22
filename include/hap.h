#ifndef __HAP_INCLUDED__
#define __HAP_INCLUDED__

#ifdef OS_Windows
#include <Windows.h>
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef long double HAPTime;
typedef struct timeState timeState;
typedef struct HAPEngine HAPEngine;


enum HAPLogLevel {
    LOGLEVEL_ERROR   = 5,
    LOGLEVEL_WARNING = 10,
    LOGLEVEL_NOTICE  = 20,
    LOGLEVEL_INFO    = 30,
};


typedef enum HAPLogLevel HAPLogLevel;

struct timeState {
    HAPTime currentTime;
    HAPTime timeDelta;

    void *timespec;
};


struct HAPEngine {
    char *name;

    timeState *time;

    HAPLogLevel logLevel;

    int  *argc;
    char **argvp;

    void (*log)(HAPEngine *engine, FILE* dest, char *message, ...);
    bool (*log_info)(HAPEngine *engine, char *message, ...);
    bool (*log_notice)(HAPEngine *engine, char *message, ...);
    bool (*log_warning)(HAPEngine *engine, char *message, ...);
    bool (*log_error)(HAPEngine *engine, char *message, ...);
    void (*log_fatal_error)(HAPEngine *engine, int code, char *message, ...);
};


HAPEngine* hap_engine_create(char *name);
void* hap_module_execute(HAPEngine *engine, const short numModules, char *identifiers[]);
void hap_engine_destroy(HAPEngine* engine);


#endif
