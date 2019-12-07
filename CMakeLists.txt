#### Root CMakeLists file of the project
#### Configuration file is build for GNU/Linux Systems
#### Resort to your local CMake guide for how to configure for Windows
cmake_minimum_required (VERSION 3.15)

### General Project Settings
project (RealTimeGraphics C)
set (EXECUTABLE_NAME "rtg")
set(CMAKE_BUILD_TYPE Debug)

### Directory Structure
set (SRC_DIR "${CMAKE_SOURCE_DIR}/src")     # Source Files
set (LIB_DIR "${CMAKE_SOURCE_DIR}/lib")     # Static libraries
set (INC_DIR "${CMAKE_SOURCE_DIR}/include") # Addition include

### External dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

set (INCS ${INC_DIR} ${SDL2_INCLUDE_DIRS}) # Include directories for compiling
set (LIBS GL dl m smol ${SDL2_LIBRARIES})  # Libraries to link target against


###
### Everything below this line will not be needed to be edited in general.
###

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-unused-variable")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

### Create configuration file from CMake settings
configure_file (
  "${CMAKE_SOURCE_DIR}/cmake/config.h.in"
  "${SRC_DIR}/config.h")

### Create and Link Executable
link_directories(${LIB_DIR}) # Link targets against following libraries
include_directories(${INCS}) # Include these targets for the compiling process

# Main Source directory
add_subdirectory(src)

# Unit testing
#enable_testing()
#add_subdirectory(test)
