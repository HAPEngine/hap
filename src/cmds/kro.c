#include <stdio.h>
#include <stdlib.h>
#include <kro.h>

int main(int argc, char **argv) {
	int index = 0;

	if (argc == 0) {
		if (kmodule_execute("kro_example") == NULL) {
			printf("Coud not load module: kro_example\n");
		}
	} else {
		for (index=0; index < argc; ++index) {
			if (kmodule_execute(argv[index]) == NULL) {
				printf("Coud not load module: %s\n", argv[index]);
			}
		}
	}

	return 0;
}
