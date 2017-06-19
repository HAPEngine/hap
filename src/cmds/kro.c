#include <kro.h>


int main(int argc, char **argv) {
	int index = 0;

	if (argc == 0) {
		kmodule_execute("kro");
	} else {
		for (index=0; index < argc; ++index) {
			kmodule_execute(argv[index]);
		}
	}

	return 0;
}
