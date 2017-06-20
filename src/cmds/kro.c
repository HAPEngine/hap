#include <stdio.h>
#include <stdlib.h>
#include <kro.h>

int main(int argc, char **argv) {
	int index = 0;

	if (argc == 0) {
		printf("No modules to load. Nothing to do.\n");
		return 0;
	}

	for (index=0; index < argc; ++index)
		kmodule_execute(argv[index]);

	return 0;
}
