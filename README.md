# The C Real Time Graphics Project

![Cubes](rtg.png)

A small project for the sole purpose of learning the basics of computer graphics and bare bones C programming.

The project consists of three major parts:
* A basic software rendering engine.
* A small matrix operations library (smol, outsourced as seperate library).
* A simple application demo of the above.

I greatly appreciate any critique and pointers concerning programming style and performance or any mistakes I may have done.

## Features
* Triangle Rasterization
* Trivial Texture Buffers
* Vertex & Index Buffers
* Vertex Array Objects and Vertex Attributes
* Perspective Vertex Attribute Interpolation
* Z-buffering
* Vertex Shader
* Fragment Shader
* Texturing

## Todo
* Clipping
* Mip-Mapping
* Filtered Texture Sampling
* Geometry Anti-Aliasing

## Bugs
* Bresenheim Line Rendering

## Testing
The test framework consists of the CTest utility of CMake together with a library containing a small set of macros, called minunit. The CMakeLists.txt in the test/ directory contains definitions for test executables, corresponding to single files of collections of test cases. All test cases perform certain assertions and checks and are packed together into a single test suite. This suite is run by the test executable, returning a zero value if everything is OK. Executing _make test_ in the build/ directory will start the CTest process, which in turn executes all defined test executables. CTest will print a short list of passed - and failed - tests. Details about a particular test suite can be obtained by executing the individual test executable manually. This will print out a more detailed view of all the assertions and checks made and also which test cases in particular failed, if any.

## Libraries
The appliccation currently uses SDL2 for hosting the display output and processing input events.
The single-header-file library stb_images.h is used to load image data into textures.

## Licence
This software is published under the GNU GPLv3.
