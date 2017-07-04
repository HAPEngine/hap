## Hap

[![CircleCI](https://circleci.com/gh/HAPEngine/hap.svg?style=svg)](https://circleci.com/gh/HAPEngine/hap)


### Building

Hap uses [cmake][cmk]!


To build all modules, you will need the following libraries:

- GLEW
- OpenAL
- OpenGL
- freeglut

After cloning this repo, run `cmake .` in the project root. If everything went
well, then a Makefile should have been generated. If not, then you may need to
set some custom CMake options in order to get things moving.

Once you have CMake generating a Makefile, you should be able to run `make`. On
Linux systems, modules will end up in lib and the hap binary will exist as
bin/hap.


[cmk]: https://cmake.org "CMake"
