#include <kro.h>


int main(int argc, char **argv) {
	int index = 0;

	if (argc == 0) {
		execute_module("kro");
	} else {
		for (index=0; index < argc; ++index) {
			execute_module(argv[index]);
		}
	}

	return 0;
}
