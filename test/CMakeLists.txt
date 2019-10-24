#### CTest Units
cmake_minimum_required (VERSION 3.15)

set (BASE_FILES
  minunit.h)

add_executable(test_matrix test_matrix.c ${BASE_FILES})
target_link_libraries(test_matrix mat)
add_test(TestMATRIX test_matrix)
