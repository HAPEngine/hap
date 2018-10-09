#ifndef __HAP_H_
#define __HAP_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#if defined _WIN32 || defined __CYGWIN__
#define HAP_MODULE_EXPORT __declspec(dllexport)
#else
#if __GNUC__ >= 4
#define HAP_MODULE_EXPORT __attribute__ ((visibility ("default")))
#else
#define HAP_MODULE_EXPORT
#endif
#endif

typedef double Unit;

#ifndef HAP_TIME_DATA_TYPE
typedef float HAPTime;
#else
typedef HAP_TIME_DATA_TYPE HAPTime;
#endif

typedef struct HAPEngine HAPEngine;
typedef struct HAPSymbol HAPSymbol;
typedef struct HAPTag    HAPTag;

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
	void(*log)(HAPEngine *engine, FILE* dest, char *message, va_list arguments);
	bool(*log_debug)(HAPEngine *engine, char *message, ...);
	bool(*log_info)(HAPEngine *engine, char *message, ...);
	bool(*log_notice)(HAPEngine *engine, char *message, ...);
	bool(*log_warning)(HAPEngine *engine, char *message, ...);
	bool(*log_error)(HAPEngine *engine, char *message, ...);
	void(*log_fatal_error)(HAPEngine *engine, int code, char *message, ...);
	
	char *name;

    int  *argc;
    char **argvp;

    timeState *time;
    HAPConfiguration *configuration;

	HAPLogLevel logLevel;

	bool isRunning;
};


struct HAPConfigurationOption {
    char *key;
    char *value;
};


struct HAPConfigurationSection {
	HAPConfigurationOption **options;
    short totalOptions;
    char *name;
};


struct HAPConfiguration {
    HAPConfigurationSection **sections;
    HAPConfigurationOption **globals;
	short totalSections;
	short totalGlobals;
};


struct HAPSymbol {
    char *name;
    unsigned int count;
};


struct HAPTag {
    char *name;

    unsigned int numValues;
    void **values;
};


// Standard entry function. Most apps need no more than this.
int hap_standard_entry(char *name, int argc, char **argv);

HAPEngine* hap_engine_create(char *name, char *configuration);
void hap_engine_destroy(HAPEngine* engine);
void* hap_module_execute(HAPEngine *engine);

HAPConfiguration* hap_configuration_load(HAPEngine *engine, char *identifier);
void hap_configuration_destroy(HAPConfiguration *config);

// Functions for referencing symbols, which are unique pointers which are
// guaranteed to be the same instance for every call with a matching name.
const HAPSymbol* hap_symbol_get(char * const name);
unsigned int hap_symbol_release(char * const name);

// Functions for working with tags, which provide a way of mapping unordered
// lists of void pointers to a given tag name.
const HAPTag* hap_tag_get(char * const name);

bool hap_tag_add(char * const name, void *value);
bool hap_tag_remove(char * const name, void *value);

#endif
