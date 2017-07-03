#include <stdio.h>
#include <hap.h>


typedef struct HAPModule {
	void* (*create)(HAPEngine *engine);
	void (*load)(HAPEngine *engine, void *state, char *identifier);
	HAPTime (*update)(HAPEngine *engine, void* state);
	void (*unload)(HAPEngine *engine, void* state);
	void (*destroy)(HAPEngine *engine, void* state);

	HAPTime nextUpdate;

	char *identifier;
	void *ref;
	void *state;
} HAPModule;


/** Module lifecycle **/
HAPModule* hap_module_create(HAPEngine *engine, char *identifier);
void hap_module_load(HAPEngine *engine, HAPModule *module);
HAPTime hap_module_update(HAPEngine *engine, HAPModule *module);
void hap_module_unload(HAPEngine *engine, HAPModule *module);
void hap_module_destroy(HAPEngine *engine, HAPModule *module);
