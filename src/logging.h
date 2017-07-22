#include <hap.h>
#include <stdbool.h>


void hap_log(HAPEngine *engine, FILE* dest, char *message, ...);
bool hap_log_info(HAPEngine *engine, char *message, ...);
bool hap_log_notice(HAPEngine *engine, char *message, ...);
bool hap_log_warning(HAPEngine *engine, char *message, ...);
bool hap_log_error(HAPEngine *engine, char *message, ...);
void hap_log_fatal_error(HAPEngine *engine, int code, char *message, ...);
