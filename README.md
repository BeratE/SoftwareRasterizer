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
* Trivial Triangle Clipping
* Vertex & Index Buffers
* Texture Buffers & Interpolation
* Vertex Array Objects with Vertex Attributes
* Perspective Vertex Attribute Interpolation
* Vertex & Fragment Shader
* Z-buffering
* Texturing

## Todo
* Polygon Clipping
* Mip-Mapping
* Anisotropic Filtering
* Geometry Anti-Aliasing

## Bugs
* Bresenheim Line Rendering

## Libraries
The appliccation currently uses SDL2 for hosting the display output and processing input events.
The single-header-file library stb_images.h is used to load image data into textures.

## Licence
This software is published under the GNU GPLv3.
