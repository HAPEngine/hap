#include <hap.h>

#ifndef OS_Windows
#include <dlfcn.h>
#include <unistd.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "module.h"
#include "timer.h"


#define MODULES_PER_LIST_GROUP 16

/**
 * Avoid allowing simulating frames below 60FPS.
 *
 * If a module is slow to update, the we don't want to get into cases
 * where frames are given large amounts of time between eachother.
 *
 * For instance, an extreme example - it would be much more difficult to detect
 * all possible collisions that occured between objects in a 3D game within the
 * last 10 seconds so it makes more sense to simulate all modules one after
 * another in 16ms increments which provides less chance of missing collisions.
 *
 * Although less common, similar issues could surely occur with audio or other
 * types of systems as well.
 */
#define MINIMUM_FPS 60
#define GET_MAX_SIMULATION_FRAME_TIME(fps) ((HAPTime) (1.0 / fps))

#ifndef MAX_SIMULATION_FRAME_TIME
#define MAX_SIMULATION_FRAME_TIME GET_MAX_SIMULATION_FRAME_TIME(MINIMUM_FPS)
#endif

#ifndef MIN_SIMULATION_FRAME_TIME
#define MIN_SIMULATION_FRAME_TIME GET_MAX_SIMULATION_FRAME_TIME(64)
#endif

typedef struct moduleList moduleList;

struct moduleList {
    HAPModule    *module;

    moduleList *next;
};

HAPModule* _hap_module_update_loop(HAPEngine *engine, short numModules, HAPModule **modules) {
    short index;

    HAPTime simulatedTime;
    HAPTime simulatedTimeDelta;

    HAPTime actualTime;
    HAPTime actualTimeDelta;

    HAPTime sleepTime;
    HAPTime nextUpdateDelta;

    int x = 0;
    for (; x < 100;) {
        ++x;

        // Set simulated timings to the values that they were during the last
        // time that the timer was updated.
        simulatedTime = (*(*engine).time).currentTime;
        simulatedTimeDelta = (*(*engine).time).timeDelta;

        // Update our timer so that we know how much time has passed since the
        // previous simulation occured.
        hap_timer_update((*engine).time);

        // Store the actual time locally so that we can use *(*engine).time as
        // a cursor which allows modules to use it to get timing information.
        actualTime = (*(*engine).time).currentTime;
        actualTimeDelta = (*(*engine).time).timeDelta;

        // Assign sleepTime so that we know how many milliseconds to sleep.
        // This allows us to ensure that a maximum framerate can be set.
        sleepTime = MIN_SIMULATION_FRAME_TIME - actualTimeDelta;

        // Ensure that we don't simulate more often than we are told to
        if (actualTimeDelta < MIN_SIMULATION_FRAME_TIME) {
#ifdef OS_Window}
            Sleep(sleepTime);
#else
            // Unix usleep takes microseconds, so convert to milliseconds.
            usleep(sleepTime * 1000);
#endif

            hap_timer_update((*engine).time);

            actualTime = (*(*engine).time).currentTime;
            actualTimeDelta = (*(*engine).time).timeDelta;
        }

        while (simulatedTime < actualTime) {
            // Ensure that we never simulate faster than the maxmimum allowed
            // delta, which should provide us a reasonable way to ensure
            // physics-related and similar checks don't miss collisions.
            if (simulatedTimeDelta > MAX_SIMULATION_FRAME_TIME)
                simulatedTimeDelta = MAX_SIMULATION_FRAME_TIME;

            // Patch currentTime with simulatedTime so that modules don't
            // get bad timing information while updating the scene state.
            (*(*engine).time).currentTime = simulatedTime;
            (*(*engine).time).timeDelta = simulatedTimeDelta;

            // Run update() to continue simulating with each module
            for (index = 0; index < numModules; ++index) {
                // If the module has requested that we don't update this often,
                // honor that request and sae some time by skipping it.
                if ((*modules[index]).nextUpdate > simulatedTime) continue;

                // Get the next update time from the module.
                nextUpdateDelta = hap_module_update(engine, modules[index]);
                (*modules[index]).nextUpdate = simulatedTime + nextUpdateDelta;

                // A negative timing means "never update again", which - for
                // now - tells the engine that the module either had a fatal
                // error or is otherwise requsting that we stop the simulation
                // for one reason or another.
                if (nextUpdateDelta < 0) return modules[index];
            }

            // For each simulation of MAX_SIMULATION_FRAME_TIME size, bring
            // simulatedTime / simulatedTimeDelta one step closer to
            // actualTime.
            simulatedTime += simulatedTimeDelta;
            simulatedTimeDelta = actualTime - simulatedTime;
        }

        // Simulation is complete, so render the final scene
        for (index = 0; index < numModules; ++index) hap_module_render(engine, modules[index]);
    }

    return NULL;
}

void* hap_module_execute(HAPEngine *engine, const short numModules, char *identifiers[]) {
    short index;
    timeState *time;

    HAPModule **modules = (HAPModule**) calloc(numModules, sizeof(HAPModule*));

    for (index = 0; index < numModules; ++index) {
        (*engine).log_debug(engine, "Creating module: %s\n", identifiers[index]);
        modules[index] = hap_module_create(engine, identifiers[index]);

        // Creating a module failed, so destroy previously created ones
        if (modules[index] == NULL) {
            --index;

            for (; index >= 0; --index) {
                hap_module_destroy(engine, modules[index]);
                modules[index] = NULL;
            }

            return NULL;
        }
    }

    for (index = 0; index < numModules; ++index) {
        (*engine).log_debug(engine, "Loading module: %s\n", identifiers[index]);
        hap_module_load(engine, modules[index]);
    }


    printf("All modules loaded.\n");

    time = (*engine).time;

    _hap_module_update_loop(engine, numModules, modules);

    for (index = 0; index < numModules; ++index) hap_module_unload(engine, modules[index]);

    for (index = 0; index < numModules; ++index) hap_module_destroy(engine, modules[index]);

    return (void*) modules;
}

HAPModule* hap_module_create(HAPEngine *engine, char *identifier) {
    HAPModule *module = (HAPModule*) calloc(1, sizeof(HAPModule));

    if (module == NULL) return NULL;

    (*module).identifier = identifier;
    (*module).nextUpdate = 0;

#ifdef OS_Windows
    (*module).ref = (void*) LoadLibrary((*module).identifier);
#else
    (*module).ref = dlopen((*module).identifier, RTLD_NOW);
#endif

    if ((*module).ref == NULL) {
        (*engine).log_error(engine, "Failed to load module: %s\n", (*module).identifier);
#ifndef OS_Windows
        (*engine).log_error(engine, "Error was: %s\n\n", dlerror());
#endif

        hap_module_destroy(engine, module);
        exit(EXIT_FAILURE);
        return NULL;
    }

#ifdef OS_Windows
    (*module).create = (void* (*)(HAPEngine* engine)) GetProcAddress((*module).ref, "create");
    (*module).load = (void(*)(HAPEngine* engine, void *state, char *identifier)) GetProcAddress((*module).ref, "load");
    (*module).update = (haptime(*)(hapengine* engine, void *state)) getprocaddress((*module).ref, "update");
    (*module).render = (void(*)(hapengine* engine, void *state)) getprocaddress((*module).ref, "render");
    (*module).unload = (void(*)(HAPEngine* engine, void *state)) GetProcAddress((*module).ref, "unload");
    (*module).destroy = (void(*)(HAPEngine* engine, void *state)) GetProcAddress((*module).ref, "destroy");
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"  /** TODO: Why doesn't `pendantic` allow these casts?... **/
    (*module).create = (void* (*)(HAPEngine* engine)) dlsym((*module).ref, "create");
    (*module).load = (void (*)(HAPEngine* engine, void *state, char *identifier)) dlsym((*module).ref, "load");
    (*module).update = (HAPTime (*)(HAPEngine* engine, void *state)) dlsym((*module).ref, "update");
    (*module).render = (void (*)(HAPEngine* engine, void *state)) dlsym((*module).ref, "render");
    (*module).unload = (void (*)(HAPEngine* engine, void *state)) dlsym((*module).ref, "unload");
    (*module).destroy = (void (*)(HAPEngine* engine, void *state)) dlsym((*module).ref, "destroy");
#pragma GCC diagnostic pop
#endif

    if ((*module).create == NULL) (*engine).log_notice(engine, "Module '%s' does not export 'create'.\n", (*module).identifier);
    if ((*module).load == NULL) (*engine).log_notice(engine, "Module '%s' does not export 'load'.\n", (*module).identifier);
    if ((*module).update == NULL) (*engine).log_notice(engine, "Module '%s' does not export 'update'\n", (*module).identifier);
    if ((*module).unload == NULL) (*engine).log_notice(engine, "Module '%s' does not export 'unload'\n", (*module).identifier);
    if ((*module).destroy == NULL) (*engine).log_notice(engine, "Module '%s' does not export 'destroy'\n", (*module).identifier);

    if ((*module).create != NULL)
        (*module).state = (*module).create(engine);

    return module;
}

void hap_module_load(HAPEngine *engine, HAPModule *module) {
    if ((*module).load == NULL) return;
    (*module).load(engine, (*module).state, (*module).identifier);
}

HAPTime hap_module_update(HAPEngine *engine, HAPModule *module) {
    if ((*module).update == NULL) return 0;
    return (*module).update(engine, (*module).state);
}

void hap_module_unload(HAPEngine *engine, HAPModule *module) {
    if ((*module).unload == NULL) return;
    (*module).unload(engine, (*module).state);
}

void _module_close_ref(void* ref) {
    if (ref == NULL) return;

#ifndef OS_Windows
    dlclose(ref);
#endif
}

void hap_module_render(HAPEngine *engine, HAPModule *module) {
    if ((*module).render == NULL) return 0;
    return (*module).render(engine, (*module).state);
}

void hap_module_destroy(HAPEngine *engine, HAPModule *module) {
    if ((*module).destroy != NULL)
        (*module).destroy(engine, (*module).state);

    _module_close_ref((*module).ref);

    (*module).state   = NULL;
    (*module).ref = NULL;

    free(module);
}
