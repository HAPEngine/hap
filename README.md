## Hap

[![CircleCI](https://circleci.com/gh/HAPEngine/hap.svg?style=svg)](https://circleci.com/gh/HAPEngine/hap)


### Building

Hap uses [cmake][_cmk]!


To build all modules, you will need the following libraries:

- [OpenAL][_oal]
- [OpenGL][_ogl]

Additionally, macOS users will also need the following libaries:

- [SDL][_sdl]

After cloning this repo, run `cmake .` in the project root. If everything went
well, then a Makefile should have been generated. If not, then you may need to
set some custom CMake options in order to get things moving.

Once you have CMake generating a Makefile, you should be able to run `make`. On
Linux systems, modules will end up in lib and the hap binary will exist as
bin/hap.


[_cmk]: https://cmake.org "CMake"
[_oal]: https://openal.org "Open Audio Library"
[_ogl]: https://opengl.org "Open Grahics Library"
[_sdl]: https://www.libsdl.org/ "Simple DirectMedia Layer"
