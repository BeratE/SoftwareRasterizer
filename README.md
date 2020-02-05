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

## Libraries
The appliccation currently uses SDL2 for hosting the display output and processing input events.
The single-header-file library stb_images.h is used to load image data into textures.

## Licence
This software is published under the GNU GPLv3.
