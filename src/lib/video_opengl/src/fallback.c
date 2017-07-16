#ifdef OS_Darwin


/***
 * NOTE: This is written in Objective-C because MacOS makes me sad
 **/

#include <hap.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "window_management.h"


struct FallbackWindow {
  SDL_Window *ref;
};

typedef struct FallbackWindow FallbackWindow;

int is_window_event(void* userData, SDL_Event *event) {
  return (*event).type == SDL_WINDOWEVENT;
}

void* window_create(HAPEngine *engine, const int width, const int height) {
  FallbackWindow *window;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Could not initialize SDL video.\n");
    return NULL;
  }

  atexit(SDL_Quit);

  window = (FallbackWindow*) calloc(1, sizeof(FallbackWindow));
  if (window == NULL) {
    fprintf(stderr, "Failed to .\n");
    return NULL;
  }

  (*window).ref = SDL_CreateWindow(
	(*engine).name,
	SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOWPOS_UNDEFINED,
	width,
	height,
	SDL_WINDOW_OPENGL
  );

  if ((*window).ref == NULL) {
    fprintf(stderr, "Failed to create SDL window.");
    return NULL;
  }

  SDL_ShowWindow((*window).ref);

  return (void*) window;
}

int window_update(void* state) {
  SDL_Event event;

  // We only care about window events in this driver
  SDL_SetEventFilter(is_window_event, NULL);

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_WINDOWEVENT:
	if (event.window.event == SDL_WINDOWEVENT_CLOSE) return -1;
    }
  }

  // Clear the event filter so that we don't interfere with other modules
  SDL_SetEventFilter(NULL, NULL);

  return 0;
}

void window_render(void* state) {
}

void window_close(void* state) {
  FallbackWindow *window = (FallbackWindow*) state;
  if ((*window).ref != NULL) SDL_DestroyWindow((*window).ref);
  free(state);

  SDL_Quit();
}


#endif
