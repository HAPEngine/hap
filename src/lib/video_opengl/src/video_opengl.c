/**
 * OpenGL module driver for HAP.
 */

#ifdef OS_Windows

#elif OS_Linux
#include "x11.h"
#endif

#include <stdlib.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include <hap.h>

#include "x11.h"


GLXContext* create(HAPEngine *engine) {
	(void)engine; // Mark variable as used to avoid compiler warnings

	if (create_window(640, 480) == NULL) {
		fprintf(stderr, "Could not create an X11 window.\n");
		return NULL;
	}

	return NULL;
}

void load(HAPEngine *engine, void *state, char *identifier) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);

	(void)engine;      // Mark variable as used to avoid compiler warnings
	(void)state;       // Mark variable as used to avoid compiler warnings
	(void)identifier;  // Mark variable as used to avoid compiler warnings
}

HAPTime update(HAPEngine *engine, void *state) {
	(void)engine;      // Mark variable as used to avoid compiler warnings
	(void)state;       // Mark variable as used to avoid compiler warnings

	return 1;
}

void unload(HAPEngine *engine, void *state) {
	(void)engine;      // Mark variable as used to avoid compiler warnings
	(void)state;       // Mark variable as used to avoid compiler warnings
}

void destroy(HAPEngine *engine, void *state) {
	(void)engine;      // Mark variable as used to avoid compiler warnings
	(void)state;       // Mark variable as used to avoid compiler warnings
}
