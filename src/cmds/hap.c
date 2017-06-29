#include <stdio.h>
#include <stdlib.h>
#include <hap.h>

int main(int argc, char **argv) {
	KSystem *system = calloc(1, sizeof(KSystem));
	short index = 0;

	if (argc <= 1) {
		printf("No modules to load. Nothing to do.\n");
		return 0;
	}

	for (index=1; index < argc; ++index)
		kmodule_execute(system, argv[index]);

	free(system);
	return 0;
}
