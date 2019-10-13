#### Root CMakeLists file of the project
#### Configuration file is build for GNU/Linux Systems
#### Resort to your local CMake guide for how to configure for Windows
cmake_minimum_required (VERSION 3.15)

### General Project Settings
project (RealTimeGraphics)
set (EXECUTABLE_NAME "rtg")

### Directory Structure
set (BIN_DIR "${CMAKE_SOURCE_DIR}/bin")     # Binary Files
set (SRC_DIR "${CMAKE_SOURCE_DIR}/src")     # Source Files
set (LIB_DIR "${CMAKE_SOURCE_DIR}/lib")     # Static libraries
set (INC_DIR "${CMAKE_SOURCE_DIR}/include") # Addition include

### External dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

set (LIBS GL dl ${SDL2_LIBRARIES})
set (INCS ${SRC_DIR} ${INC_DIR} ${SDL2_INCLUDE_DIRS})

### Configuration File Settings
## OpenGL Version Number
set (OPENGL_VERSION_MAJOR 3)
set (OPENGL_VERSION_MINOR 3)

###
### Everything below this line will not be needed to be edited in general.
###
set (EXECUTABLE_OUTPUT_PATH ${BIN_DIR})

### Source file globbing
file (GLOB_RECURSE SOURCE_FILES
  ${SRC_DIR}/*.c
  ${SRC_DIR}/*.cpp)

file (GLOB_RECURSE HEADER_FILES
  ${SRC_DIR}/*.h
  ${SRC_DIR}/*.hpp)

### Create configuration file from CMake settings
configure_file (
  "${SRC_DIR}/config.h.in"
  "${SRC_DIR}/config.h")

### Create and Link Executable
link_directories(${LIB_DIR})
include_directories(${INCS})

add_executable (${EXECUTABLE_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${EXECUTABLE_NAME} ${LIBS})
