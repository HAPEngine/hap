#include <hap.h>

#include "module.h"
#include "timer.h"


HAPEngine* hap_engine_create() {
	HAPEngine *engine = calloc(1, sizeof(HAPEngine));

	if (engine == NULL) return NULL;

	(*engine).time = hap_timer_update((*engine).time);

	return engine;
}


void hap_engine_destroy(HAPEngine *engine) {
	hap_timer_destroy((*engine).time);
	free(engine);
}
