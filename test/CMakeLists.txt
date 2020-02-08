#### CTest Units
cmake_minimum_required (VERSION 3.15)

set (BASE_FILES
  minunit.h)

include_directories(${SRE_DIR})

### Test Object Loader
add_executable(test_obloader test_objloader.c ${BASE_FILES})
target_link_libraries(test_obloader ${SRE_TGT})
add_test(TestOBJLOADER test_obloader)
