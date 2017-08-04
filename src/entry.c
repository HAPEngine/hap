#include <hap.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef OS_Windows
#define MAX_LOADABLE_MODULES 256
#endif

#ifndef _strdup
#define _strdup strdup
#endif


int hap_standard_entry(char *name, int argc, char **argv) {
    char *currentModule;
    char *configurationIdentifier;
    char **identifiers;
    int index;

    if (argc >= 2) configurationIdentifier = argv[1];
    else configurationIdentifier = name;

    HAPEngine *engine = hap_engine_create(NULL, configurationIdentifier);

    if (engine == NULL) return 1;

    identifiers = calloc((*(*engine).configuration).totalSections, sizeof(char*));

    (*engine).argc = &argc;
    (*engine).argvp = argv;

    for (index = 0; index < (*(*engine).configuration).totalSections; ++index) {
        currentModule = (*(*(*engine).configuration).sections[index]).name;
        (*engine).log_info(engine, "Loading module: %s\n", currentModule);
        identifiers[index] = _strdup(currentModule);
    }

    hap_module_execute(engine, argc - 1, identifiers);
    hap_engine_destroy(engine);

    for (index = 0; index < (*(*engine).configuration).totalSections; ++index)
        free(identifiers[index]);

    return 0;
}
