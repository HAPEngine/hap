#ifdef OS_Windows

#include <hap.h>

#include <stdbool.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/wglext.h>

#include "window_management.h"


struct Win32Window {
	HWND            ref;

	HGLRC           renderingContext;
	HDC             deviceContext;
	HINSTANCE       application;
};


typedef struct Win32Window Win32Window;


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static PAINTSTRUCT ps;

	switch (uMsg) {
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		window_render(NULL);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_SIZE:
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		PostMessage(hWnd, WM_PAINT, 0, 0);
		return 0;

	case WM_CHAR:
		switch (wParam) {
		case 27:
			PostQuitMessage(0);
			break;
		}
		return 0;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void* window_create(const int width, const int height) {
	const char* windowClassName = "OpenGL";
	const char* title = "hap";

	int         pf;
	WNDCLASS    wc;
	PIXELFORMATDESCRIPTOR pfd;

	Win32Window* window = calloc(1, sizeof(Win32Window));

	if (window == NULL) {
		fprintf(stderr, "Could not allocate memory for window. You may be out of RAM.");
		return NULL;
	}

	(*window).application = GetModuleHandle(NULL);

	wc.style = CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = (*window).application;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "OpenGL";

	if (!RegisterClass(&wc)) {
		fprintf(stderr, "RegisterClass() failed: Cannot register window class.");
		return NULL;
	}

	(*window).ref = CreateWindow(
		windowClassName,
		title,
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		10, 10,
		width, height,
		NULL, NULL,
		(*window).application,
		NULL
	);

	if ((*window).ref == NULL) {
		fprintf(stderr, "CreateWindow() failed:  Cannot create a window.");
		return NULL;
	}

	(*window).deviceContext = GetDC((*window).ref);

	/* there is no guarantee that the contents of the stack that become
	the pfd are zeroed, therefore _make sure_ to clear these bits. */
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	pfd.iPixelType = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	pfd.cColorBits = 32;

	pf = ChoosePixelFormat((*window).deviceContext, &pfd);
	if (pf == 0) {
		fprintf(stderr, "Could not find a suitable pixel format.");
		return 0;
	}

	if (SetPixelFormat((*window).deviceContext, pf, &pfd) == FALSE) {
		fprintf(stderr, "Could not the expected pixel format.");
		return 0;
	}

	DescribePixelFormat((*window).deviceContext, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	ReleaseDC((*window).ref, (*window).deviceContext);

	(*window).deviceContext = GetDC((*window).ref);
	(*window).renderingContext = wglCreateContext((*window).deviceContext);

	ShowWindow((*window).ref, SW_SHOWNORMAL);

	return (void*) window;
}


int window_update(void* state) {
	MSG   msg;
	Win32Window *window = (Win32Window*)state;

	wglMakeCurrent((*window).deviceContext, (*window).renderingContext);

	while (GetMessage(&msg, (*window).ref, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


void window_render(void* state) {}


void window_close(void* state) {
	Win32Window *window = (Win32Window*)state;

	wglMakeCurrent(NULL, NULL);
	ReleaseDC((*window).ref, (*window).deviceContext);
	wglDeleteContext((*window).renderingContext);

	DestroyWindow((*window).ref);
}


#endif