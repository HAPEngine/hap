#ifdef OS_Linux


#include <stdio.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include <X11/X.h>
#include <X11/Xlib.h>

#include <hap.h>

#include "x11.h"


struct GLWindow {
	Display                *display;
	Window                 root;
	Colormap               colormap;
	Window                 ref;
	XWindowAttributes      attributes;
	XEvent                 xev;
	GLXContext             glContext;
};


typedef struct GLWindow GLWindow;


GLint glAttributes[] = {
	GLX_RGBA, GLX_DEPTH_SIZE, 24,
	GLX_DOUBLEBUFFER, None
};


void* window_create(const int width, const int height) {
	GLWindow* window = (GLWindow*) calloc(1, sizeof(GLWindow*));
	XSetWindowAttributes setAttributes;
	XVisualInfo            *visualInfo;

	if (window == NULL) {
		fprintf(stderr, "Could not allocate memory to create a window.");
		return NULL;
	}

	(*window).display = XOpenDisplay(NULL);

	// Occurs when the XServer can't be connected to
	if ((*window).display == NULL) {
		fprintf(stderr, "Could not open display. Is there an XServer running?\n");
		return NULL;
	}

	(*window).root = DefaultRootWindow((*window).display);

	if (!(*window).root) {
		fprintf(stderr, "Could not find root window.\n");
		(*window).root = DefaultRootWindow((*window).display);
	}

	visualInfo = glXChooseVisual((*window).display, 0, glAttributes);

	if (visualInfo == NULL) {
		fprintf(stderr, "Couldn't get GLX VisualInfo for XServer.\n");
		return NULL;
	}

	setAttributes.event_mask = ExposureMask | KeyPressMask;
	setAttributes.colormap = XCreateColormap(
	                             (*window).display, (*window).root,
	                             (*visualInfo).visual,
	                             AllocNone
	                         );


	(*window).ref = XCreateWindow(
	                    (*window).display, (*window).root,
	                    10, 10, width, height, 0,
	                    (*visualInfo).depth,
	                    InputOutput,
	                    (*visualInfo).visual,
	                    CWColormap | CWEventMask,
	                    &setAttributes
	                );

	XMapWindow((*window).display, (*window).ref);
	XStoreName((*window).display, (*window).ref, "HAP");

	(*window).glContext = glXCreateContext(
	                          (*window).display, visualInfo,
	                          NULL, GL_TRUE
	                      );

	glXMakeCurrent(
	    (*window).display, (*window).ref,
	    (*window).glContext
	);

	return (void*) window;
}


void window_update(void* window) {
	GLWindow* glw = (Window*) window;
	glXMakeCurrent((*glw).display, (*glw).ref, (*glw).glContext);
}


void window_close(void* window) {
}


#endif
