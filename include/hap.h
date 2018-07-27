#ifndef __HAP_H_
#define __HAP_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


typedef double Unit;
typedef float HAPTime;

typedef struct HAPEngine HAPEngine;
typedef struct timeState timeState;

typedef struct HAPConfiguration HAPConfiguration;
typedef struct HAPConfigurationOption  HAPConfigurationOption;
typedef struct HAPConfigurationSection HAPConfigurationSection;
typedef union  HAPConfigurationValue   HAPConfigurationValue;


typedef enum {
    LOGLEVEL_FATAL   = 0,
    LOGLEVEL_ERROR   = 5,
    LOGLEVEL_WARNING = 10,
    LOGLEVEL_NOTICE  = 20,
    LOGLEVEL_INFO    = 30,
    LOGLEVEL_DEBUG   = 50,
} HAPLogLevel;


typedef enum {
    HAP_CONFIGURATION_BOOL = 0,
    HAP_CONFIGURATION_DOUBLE = 1,
    HAP_CONFIGURATION_FLOAT = 2,
    HAP_CONFIGURATION_STRING = 3,

    HAP_CONFIGURATION_UNIT = 10,
} HAPConfigurationValueType;


struct timeState {
    HAPTime currentTime;
    HAPTime timeDelta;

    void *timespec;
};


struct HAPEngine {
    char *name;

    int  *argc;
    char **argvp;

    timeState *time;
    HAPLogLevel logLevel;
    HAPConfiguration *configuration;

    void (*log)(HAPEngine *engine, FILE* dest, char *message, va_list arguments);
    bool (*log_debug)(HAPEngine *engine, char *message, ...);
    bool (*log_info)(HAPEngine *engine, char *message, ...);
    bool (*log_notice)(HAPEngine *engine, char *message, ...);
    bool (*log_warning)(HAPEngine *engine, char *message, ...);
    bool (*log_error)(HAPEngine *engine, char *message, ...);
    void (*log_fatal_error)(HAPEngine *engine, int code, char *message, ...);
};


struct HAPConfigurationOption {
    char *key;
    char *value;
};


struct HAPConfigurationSection {
    char *name;

    short totalOptions;
    HAPConfigurationOption **options;
};


struct HAPConfiguration {
    short totalSections;
    HAPConfigurationSection **sections;

    short totalGlobals;
    HAPConfigurationOption **globals;
};

// Standard entry function. Most apps need no more than this.
int hap_standard_entry(char *name, int argc, char **argv);

HAPEngine* hap_engine_create(char *name, char *configuration);
void hap_engine_destroy(HAPEngine* engine);
void* hap_module_execute(HAPEngine *engine);

HAPConfiguration* hap_configuration_load(HAPEngine *engine, char *identifier);
void hap_configuration_destroy(HAPConfiguration *config);

#endif
