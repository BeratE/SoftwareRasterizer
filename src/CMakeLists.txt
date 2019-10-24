cmake_minimum_required (VERSION 3.15)

### Software Rendering Engine library files
set (SRE_FILES
  # Header
  ${SRC_DIR}/colorbuffer.h
  ${SRC_DIR}/pipeline.h
  ${SRC_DIR}/rasterizer.h
  # Source
  ${SRC_DIR}/colorbuffer.c
  ${SRC_DIR}/pipeline.c
  ${SRC_DIR}/rasterizer.c)

### Matrix Library files
set (MAT_FILES
  ${SRC_DIR}/matrix/matrix.h
  ${SRC_DIR}/matrix/matrix.c)

### Application files
set (APP_FILES
  ${SRC_DIR}/main.c
  ${SRC_DIR}/config.h)


add_library(mat ${MAT_FILES})
add_library(sre ${SRE_FILES})

add_executable (${EXECUTABLE_NAME} ${APP_FILES})
target_link_libraries(${EXECUTABLE_NAME} ${LIBS} mat sre)
