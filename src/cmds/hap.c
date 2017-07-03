#include <stdio.h>
#include <stdlib.h>
#include <hap.h>

int main(int argc, char **argv) {
	HAPEngine *engine = calloc(1, sizeof(HAPEngine));
	if (engine == NULL) return 1;

	(*engine).name = "hap";
	(*engine).argc = &argc;
	(*engine).argvp = argv;

	short index = 0;

	if (argc <= 1) {
		printf("No modules to load. Nothing to do.\n");
		return 0;
	}

	for (index=1; index < argc; ++index)
		hap_module_execute(engine, argv[index]);

	free(engine);
	return 0;
}
