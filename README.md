# The C Real Time Graphics Project

A simple project for the sole purpose of learning the basics of computer graphics and bare bones C programming.

The project consists of three major parts:
* A basic software rendering engine.
* A small matrix operations library (smol).
* A simple application demo of the above.

I greatly appreciate any critique and pointers concerning programming style and performance or any other mistakes I may have done (does the _const_ qualifier make any sense on pointer parameters? Should I store the matrix in a fixed sized array or an allocated pointer? Does it make any difference? How should I access the matrix elements to prevent page faults? etc.)

## Features
* Triangle Rasterization
* Trivial Texture Buffers
* Vertex & Index Buffers
* Vertex Array Objects and Vertex Attributes
* Vertex Shader

## Todo
* Fragment Shader
* Z-buffering
* Clipping
* Texturing
* Geometry Anti-Aliasing
* Fix Line Rendering Algorithm

## Testing
The test framework consists of the CTest utility of CMake together with a small set of macros called minunit. 

The CMakeLists.txt in the test/ directory contains definitions for test executables, corresponding to single files of collections of test cases. All test cases perform certain assertions and checks and are packed together into a single test suite. This suite is run by the test executable and Ig everything is OK, a zero value is returned. A non-zero value indicates a failure to the CTest environment.

Executing _make test_ in the build/ directory will start the CTest process, which in turn executes all defined test executables. CTest will print a short list of passed - and failed - tests. Details about a particular test suite can be obtained by executing the individual test executable manually. This will print out a more detailed view of all the assertions and checks made and which test cases in particular failed, if any.

## Libraries
The appliccation currently uses SDL2 for displaying the rendered colorbuffer onto the screen.

## Licence
This software is published under the GNU GPLv3.
