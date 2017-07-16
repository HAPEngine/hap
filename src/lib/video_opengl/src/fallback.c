#ifdef OS_Darwin


/***
 * NOTE: This is written in Objective-C because MacOS makes me sad
 **/

#include <hap.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>

#include "window_management.h"


struct SDLWindow {
  SDL_Surface *screen;
};

typedef struct SDLWindow SDLWindow;

void* window_create(HAPEngine *engine, const int width, const int height) {
  SDLWindow *window;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Could not initialize SDL video.\n");
    return NULL;
  }

  atexit(SDL_Quit);

  window = (SDLWindow*) calloc(1, sizeof(SDLWindow));
  if (window == NULL) {
    fprintf(stderr, "Failed to .\n");
    return NULL;
  }

  (*window).screen = SDL_SetVideoMode(width, height, 8, SDL_SWSURFACE);

  if ((*window).screen == NULL) {
    fprintf(stderr, "Failed to set video mode.\n");
    return NULL;
  }

  return (void*) window;
}

int window_update(void* state) {
  return 0;
}

void window_render(void* state) {
}

void window_close(void* state) {
  SDLWindow *window = (SDLWindow*) state;
  free(state);
}


#endif
