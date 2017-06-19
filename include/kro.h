#ifndef __KRO_LIB__
#define __KRO_LIB__


#include <stdio.h>


#define DEBUG(text)
	#ifdef _DEBUG
	/* printf("%s\n", text); */
	#endif


typedef struct KModule {
	char *identifier;
	void *ref;
	void *state;

	void* (*create)(void);
	void (*load)(char *identifier);
	void (*update)(void* state);
	void (*unload)(void* state);
	void (*destroy)(void* state);
} KModule;


KModule* execute_module(char *identifier);


#endif
