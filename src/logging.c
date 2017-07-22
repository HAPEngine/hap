#include <hap.h>
#include <stdio.h>

#include "logging.h"


bool hap_log_necessary(HAPEngine *engine, HAPLogLevel level) {
    return (*engine).logLevel < level;
}


void hap_log(HAPEngine *engine, FILE *dest, char *message, ...) {
    char *strBuffer = calloc(sprintf(NULL, "%s", message), sizeof(char));

    // TODO: Handle error?...
    if (strBuffer == NULL) return;

    sprintf(strBuffer, "%s", message);
    fprintf(dest, "[%s] %s", (*engine).name, message);
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
