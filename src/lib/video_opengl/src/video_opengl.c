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


void* create(HAPEngine *engine) {
	(void)engine; // Mark variable as used to avoid compiler warnings

	void *window = window_create(640, 480);

	if (window == NULL) {
		fprintf(stderr, "Could not create a window.\n");
		return NULL;
	}

	return window;
}

void load(HAPEngine *engine, void *state, char *identifier) {
	glEnable(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);

	(void)engine;      // Mark variable as used to avoid compiler warnings
	(void)state;       // Mark variable as used to avoid compiler warnings
	(void)identifier;  // Mark variable as used to avoid compiler warnings
}

HAPTime update(HAPEngine *engine, void *state) {
	(void)engine;      // Mark variable as used to avoid compiler warnings

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (window_update(state) >= 0) {
		return 0.1;

	} else {
		return -1;
	}
}

void unload(HAPEngine *engine, void *state) {
	(void)engine;      // Mark variable as used to avoid compiler warnings
	(void)state;       // Mark variable as used to avoid compiler warnings
}

void destroy(HAPEngine *engine, void *state) {
	(void)engine;      // Mark variable as used to avoid compiler warnings

	window_close(state);
}
