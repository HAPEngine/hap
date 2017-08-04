#ifndef __HAP_INCLUDED__
#define __HAP_INCLUDED__

#ifdef OS_Windows
#include <Windows.h>
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


typedef double Unit;
typedef long double HAPTime;

typedef struct HAPEngine HAPEngine;
typedef struct timeState timeState;

typedef struct HapConfiguration HapConfiguration;
typedef struct HapConfigurationOption  HapConfigurationOption;
typedef struct HapConfigurationSection HapConfigurationSection;
typedef union  HapConfigurationValue   HapConfigurationValue;


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
} HapConfigurationValueType;


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
    HapConfiguration *configuration;

    void (*log)(HAPEngine *engine, FILE* dest, char *message, ...);
    bool (*log_debug)(HAPEngine *engine, char *message, ...);
    bool (*log_info)(HAPEngine *engine, char *message, ...);
    bool (*log_notice)(HAPEngine *engine, char *message, ...);
    bool (*log_warning)(HAPEngine *engine, char *message, ...);
    bool (*log_error)(HAPEngine *engine, char *message, ...);
    void (*log_fatal_error)(HAPEngine *engine, int code, char *message, ...);
};


struct HapConfigurationOption {
    char *key;
    char *value;
};


struct HapConfigurationSection {
    char *name;

    short totalOptions;
    HapConfigurationOption **options;
};


struct HapConfiguration {
    short totalSections;
    HapConfigurationSection **sections;

    short totalGlobals;
    HapConfigurationOption **globals;
};

// Standard entry function. Most apps need no more than this.
int hap_standard_entry(char *name, int argc, char **argv);

HAPEngine* hap_engine_create(char *name, char *configuration);
void hap_engine_destroy(HAPEngine* engine);
void* hap_module_execute(HAPEngine *engine, const short numModules, char *identifiers[]);

HapConfiguration* hap_configuration_load(HAPEngine *engine, char *identifier);
void hap_configuration_destroy(HapConfiguration *config);

#endif
