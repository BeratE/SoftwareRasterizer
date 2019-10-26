#### CTest Units
cmake_minimum_required (VERSION 3.15)

set (BASE_FILES
  minunit.h)

include_directories(${SRC_DIR})

### Test Matrix Library
add_executable(test_matrix test_matrix.c ${BASE_FILES})
target_link_libraries(test_matrix smol)
add_test(TestMATRIX test_matrix)
