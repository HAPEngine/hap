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
	XVisualInfo            *visualInfo;
	Colormap               colormap;
	XSetWindowAttributes   setAttributes;
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


void* create_window(const int width, const int height) {
	GLWindow* window = (GLWindow*) calloc(1, sizeof(GLWindow*));

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
	(*window).visualInfo = glXChooseVisual((*window).display, 0, glAttributes);


	if ((*window).visualInfo == NULL) {
		fprintf(stderr, "Couldn't get GLX VisualInfo for XServer.\n");
		return NULL;
	}

	Colormap colormap = XCreateColormap(
	                        (*window).display, (*window).root,
	                        (*(*window).visualInfo).visual,
	                        AllocNone
	                    );

	(*window).setAttributes.event_mask = ExposureMask | KeyPressMask;
	(*window).setAttributes.colormap = colormap;


	(*window).ref = XCreateWindow(
	                    (*window).display, (*window).root,
	                    0, 0, width, height, 0,
	                    (*(*window).visualInfo).depth,
	                    InputOutput,
	                    (*(*window).visualInfo).visual,
	                    CWColormap | CWEventMask,
	                    &(*window).setAttributes
	                );

	/* XMapWindow((*window).display, (*window).ref); */
	/* XStoreName((*window).display, (*window).ref, "HAP"); */

	glXMakeCurrent(
	    (*window).display, (*window).ref,
	    (*window).glContext
	);

	return (void*) window;
}


#endif
