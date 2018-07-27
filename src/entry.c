#include <hap.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef OS_Windows
#define MAX_LOADABLE_MODULES 256
#endif


int hap_standard_entry(char *name, int argc, char **argv) {
    char *currentModule;
    char *configurationIdentifier;
    int index;

    if (argc > 1) configurationIdentifier = argv[1];
    else configurationIdentifier = name;

    HAPEngine *engine = hap_engine_create(NULL, configurationIdentifier);

    if (engine == NULL) return 1;

    (*engine).argc = &argc;
    (*engine).argvp = argv;

    hap_module_execute(engine);
    hap_engine_destroy(engine);

    return 0;
}
