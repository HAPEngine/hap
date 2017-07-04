#ifdef OS_Linux


#include <stdbool.h>
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
	Window                 ref;
	GLXContext             glContext;
};


typedef struct GLWindow GLWindow;


Atom                   WM_DELETE_WINDOW;


GLint glAttributes[] = {
	GLX_RGBA, GLX_DEPTH_SIZE, 24,
	GLX_DOUBLEBUFFER, None
};


void* window_create(const int width, const int height) {
	GLWindow* window = (GLWindow*) calloc(1, sizeof(GLWindow*));
	Window root;
	XSetWindowAttributes setAttributes;
	XVisualInfo *visualInfo;

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

	root = DefaultRootWindow((*window).display);

	if (!root) {
		fprintf(stderr, "Could not find root window.\n");
		root = DefaultRootWindow((*window).display);
	}

	visualInfo = glXChooseVisual((*window).display, 0, glAttributes);

	if (visualInfo == NULL) {
		fprintf(stderr, "Couldn't get GLX VisualInfo for XServer.\n");
		return NULL;
	}

	setAttributes.event_mask = ExposureMask | KeyPressMask;
	setAttributes.colormap = XCreateColormap(
	                             (*window).display, root,
	                             (*visualInfo).visual,
	                             AllocNone
	                         );


	(*window).ref = XCreateWindow(
	                    (*window).display, root,
	                    10, 10, width, height, 0,
	                    (*visualInfo).depth,
	                    InputOutput,
	                    (*visualInfo).visual,
	                    CWColormap | CWEventMask,
	                    &setAttributes
	                );

	XMapWindow((*window).display, (*window).ref);
	WM_DELETE_WINDOW = XInternAtom((*window).display, "WM_DELETE_WINDOW", false);
	XSetWMProtocols((*window).display, (*window).ref, &WM_DELETE_WINDOW, 1);

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


int window_update(void* state) {
	XEvent event;

	GLWindow *window = (GLWindow*) state;

	if (window == NULL) {
		return -1;
	}

	while (XPending((*window).display)) {
		XNextEvent((*window).display, &event);

		if (event.type == ClientMessage) {
			if ((Atom)event.xclient.data.l[0] == WM_DELETE_WINDOW) {
				return -1;
			}
		}
	}

	return 0;
}


void window_close(void* state) {
	GLWindow *window = (GLWindow*) state;

	glXMakeCurrent((*window).display, None, NULL);
	glXDestroyContext((*window).display, (*window).glContext);
	(*window).glContext = 0;

	XDestroyWindow((*window).display, (*window).ref);
	XCloseDisplay((*window).display);

	free(window);
}


#endif
