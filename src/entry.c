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
    char *configurationIdentifier;
    int index;

    if (argc >= 2) configurationIdentifier = argv[1];
    else configurationIdentifier = name;

    HAPEngine *engine = hap_engine_create(NULL, configurationIdentifier);

    if (engine == NULL) return 1;

    (*engine).argc = &argc;
    (*engine).argvp = argv;

    /* for (index = 1; index < argc; ++index) { */
    /*     (*engine).log_info(engine, "Loading module: %s\n", argv[index]); */
    /*     identifiers[index-1] = _strdup(argv[index]); */
    /* } */

    /* hap_module_execute(engine, argc - 1, identifiers); */
    /* hap_engine_destroy(engine); */

    /*     for (index = 0; index < argc-1; ++index) free(identifiers[index]); */

    return 0;
}
