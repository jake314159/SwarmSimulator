cmake_minimum_required(VERSION 2.6)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

INCLUDE(CMakeForceCompiler) 
CMAKE_FORCE_C_COMPILER(gcc GNU)
CMAKE_FORCE_CXX_COMPILER(g++ GNU)
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests Vector2d_test.cpp Point2d_test.cpp ../gtest-1.7.0/make/sample1_unittest)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)

