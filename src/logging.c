#include <hap.h>
#include <stdarg.h>
#include <stdio.h>

#include "logging.h"


bool hap_log_necessary(HAPEngine *engine, HAPLogLevel level) {
    return (*engine).logLevel <= level;
}


void hap_log(HAPEngine *engine, FILE *dest, char *message, ...) {
    va_list arguments, argumentsCopy;

    // The first call to vsprintf gets the length of the resulting string
    va_start(arguments, message);
    va_copy(argumentsCopy, arguments);
    size_t length = vsnprintf(NULL, 0, message, argumentsCopy);
    va_end(argumentsCopy);

    char *strBuffer = calloc(length, sizeof(char));

    if (strBuffer == NULL) exit(500);

    // Second call to vsprintf for formatting the given message string
    vsnprintf(strBuffer, length, message, arguments);
    va_end(arguments);

    // Finally, output the result with the prefix to dest
    fprintf(dest, "[%s] %s", (*engine).name, message);

    free(strBuffer);
}


bool hap_log_info(HAPEngine *engine, char *message, ...) {
    if (!hap_log_necessary(engine, LOGLEVEL_INFO)) return false;
    hap_log(engine, stdout, message);
    return true;
}


bool hap_log_notice(HAPEngine *engine, char *message, ...) {
    if (!hap_log_necessary(engine, LOGLEVEL_NOTICE)) return false;
    hap_log(engine, stdout, message);
    return true;
}


bool hap_log_warning(HAPEngine *engine, char *message, ...) {
    if (!hap_log_necessary(engine, LOGLEVEL_WARNING)) return false;
    hap_log(engine, stderr, message);
    return true;
}


bool hap_log_error(HAPEngine *engine, char *message, ...) {
    if (!hap_log_necessary(engine, LOGLEVEL_ERROR)) return false;
    hap_log(engine, stderr, message);
    return true;
}


void hap_log_fatal_error(HAPEngine *engine, int code, char *message, ...) {
    hap_log(engine, stderr, message);
    exit(code);
}
