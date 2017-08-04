#include <hap.h>
#include <stdarg.h>
#include <stdio.h>

#include "logging.h"


#define LOGLEVEL(fn, constant, io) bool fn(HAPEngine *engine, char *message, ...) { \
    va_list arguments;                                                                      \
    va_start(arguments, message);                                                           \
    if (!hap_log_necessary(engine, constant)) return false;                      \
    hap_log(engine, io, message, arguments);                                                \
    va_end(arguments);                                                                      \
    return true;                                                                            \
}


bool hap_log_necessary(HAPEngine *engine, HAPLogLevel level) {
    return (*engine).logLevel >= level;
}


void hap_log(HAPEngine *engine, FILE *dest, char *message, va_list arguments) {
    char *strBuffer;
    int length;
    va_list checkLengthArguments;

    // The first call to vsprintf gets the length of the resulting string
    va_copy(checkLengthArguments, arguments);

    length = vsnprintf(NULL, 0, message, checkLengthArguments);

    if (length < 0) exit(500);

    strBuffer = calloc(length+1, sizeof(char));

    if (strBuffer == NULL) exit(510);

    // Second call to vsprintf for formatting the given message string
    length = vsnprintf(strBuffer, length+1, message, arguments);

    if (length < 0) exit(505);

    // Finally, output the result with the prefix to dest
    fprintf(dest, "[%s] %s\n", (*engine).name, strBuffer);

    free(strBuffer);
}


LOGLEVEL(hap_log_debug, LOGLEVEL_DEBUG, stdout)
LOGLEVEL(hap_log_info, LOGLEVEL_INFO, stdout)
LOGLEVEL(hap_log_notice, LOGLEVEL_NOTICE, stdout)
LOGLEVEL(hap_log_warning, LOGLEVEL_WARNING, stderr)
LOGLEVEL(hap_log_error, LOGLEVEL_ERROR, stderr)


void hap_log_fatal_error(HAPEngine *engine, int code, char *message, ...) {
    va_list arguments;
    va_start(arguments, message);
    if (hap_log_necessary(engine, LOGLEVEL_FATAL)) hap_log(engine, stderr, message, arguments);
    va_end(arguments);
    exit(code);
}
