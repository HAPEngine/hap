#include <hap.h>
#include <stdio.h>


#define MAX_LOG_BUFFER_SIZE 256


void hap_log(HAPEngine *engine, HAPLogLevel level, char *message) {
	if ((*engine).loglevel >= level) return;

	printf("[%s] %s\n", (*engine).name, message);
}
