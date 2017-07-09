#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hap.h>

#ifdef OS_Windows
#define MAX_LOADABLE_MODULES 256
#endif

#ifndef _strdup
#define _strdup strdup
#endif

int main(int argc, char **argv) {
	int index;

#ifdef OS_Windows
	char *identifiers[MAX_LOADABLE_MODULES];
#else
	char *identifiers[argc - 1];
#endif

	HAPEngine *engine = hap_engine_create();

	if (engine == NULL) return 1;

	(*engine).name = "hap";
	(*engine).argc = &argc;
	(*engine).argvp = argv;

	if (argc <= 1) {
		printf("No modules to load. Nothing to do.\n");
		return 0;
	}

	for (index = 1; index < argc; ++index) {
		identifiers[index-1] = _strdup(argv[index]);
	}

	hap_module_execute(engine, argc - 1, identifiers);
	hap_engine_destroy(engine);

	for (index = 0; index < argc-1; ++index) free(identifiers[index]);

	return 0;
}
