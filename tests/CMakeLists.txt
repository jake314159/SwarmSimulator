cmake_minimum_required(VERSION 2.6)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests Vector2d_test.cpp Point2d_test.cpp)
add_executable(runTests_agent Agent_test.cpp)

target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
target_link_libraries(runTests_agent ${GTEST_LIBRARIES} pthread)


