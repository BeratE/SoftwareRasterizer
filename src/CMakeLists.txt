#### Source Cmake file
cmake_minimum_required (VERSION 3.15)


### Software Rendering Engine library files
set (SRE_TGT sre) # Name of target library
set (SRE_DIR "${SRC_DIR}/sre")
set (SRE_FILES
  # Header
  ${SRE_DIR}/texturebuffer.h
  ${SRE_DIR}/rasterizer.h
  ${SRE_DIR}/pipeline.h
  # Source
  ${SRE_DIR}/texturebuffer.c
  ${SRE_DIR}/rasterizer.c
  ${SRE_DIR}/pipeline.c)


### Matrix Library files
### Simple Matrix Operations Library (smol)
set (MAT_TGT smol) # Name of target library
set (MAT_DIR "${SRC_DIR}/smol")
set (MAT_FILES
  # Header
  ${MAT_DIR}/matrix.h
  # Source
  ${MAT_DIR}/matrix.c)


### Application files
set (APP_FILES
  # Header
  ${SRC_DIR}/config.h
  # Source
  ${SRC_DIR}/main.c)


### Target libraries
add_library(${MAT_TGT} ${MAT_FILES})
add_library(${SRE_TGT} ${SRE_FILES})

### Final target executable
add_executable (${EXECUTABLE_NAME} ${APP_FILES})
target_link_libraries(${EXECUTABLE_NAME} ${LIBS} ${MAT_TGT} ${SRE_TGT})
