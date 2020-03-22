#### Source Cmake file
cmake_minimum_required (VERSION 3.15)

### Software Rendering Engine library
set (SRE_TGT sre) # Name of target library
set (SRE_DIR "${SRC_DIR}/sre")
set (SRE_FILES
  # Header
  ${SRE_DIR}/sre.h
  ${SRE_DIR}/sredefs.h
  ${SRE_DIR}/sretypes.h
  # Source
  ${SRE_DIR}/sre.c
  ${SRE_DIR}/sretexbuffer.c
  ${SRE_DIR}/srerasterizer.c
  ${SRE_DIR}/sreassl.c)


### Application files
set (APP_FILES
  # Header
  ${SRC_DIR}/stb_image.h
  ${SRC_DIR}/config.h
  # Source
  ${SRC_DIR}/main.c
  ${SRC_DIR}/shader.c)

### Target libraries
add_library(${SRE_TGT} ${SRE_FILES})

### Final target executable
add_executable (${EXECUTABLE_NAME} ${APP_FILES})
target_link_libraries(${EXECUTABLE_NAME} ${LIBS} ${SRE_TGT})

# Unit testing
enable_testing()
add_subdirectory(test)
