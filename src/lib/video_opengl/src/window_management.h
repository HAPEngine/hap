#include <hap.h>

#include <GL/gl.h>
#include <GL/glu.h>


#ifdef OS_Linux
#include <GL/glx.h>
#endif


void* window_create(const int width, const int height);
int window_update(void* window);
void window_render(void* window);
void window_close(void* window);