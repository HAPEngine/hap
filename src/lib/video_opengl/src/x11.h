#ifdef OS_Linux


#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>


void* window_create(const int width, const int height);
void window_update(void* window);
void window_close(void* window);


#endif
