/** Modifies the linker so that it uses proper symbol names in MSVC. **/
#ifdef OS_Windows
#pragma comment(linker, "/EXPORT:create=create")
#pragma comment(linker, "/EXPORT:load=load")
#pragma comment(linker, "/EXPORT:update=update")
#pragma comment(linker, "/EXPORT:unload=unload")
#pragma comment(linker, "/EXPORT:destroy=destroy")
#endif

#include <Python.h>
#include <hap.h>

void* create(HAPEngine *engine) {
	Py_SetProgramName(((wchar_t**) (*engine).argvp)[0]);
	Py_Initialize();
	return NULL;
}


void load(HAPEngine *engine, void *state, char *identifier) {
	(void)engine;
	(void)identifier;
	(void)state;
}


HAPTime update(HAPEngine *engine, void *state) {
	(void)engine;
	(void)state;
	
	return 0;
}


void unload(HAPEngine *engine, void *state) {
	(void)engine;
	(void)state;
}


void destroy(HAPEngine *engine, void *state) {
	(void)state;

	Py_Finalize();
}
