# CMake lowest version requirement
cmake_minimum_required(VERSION 3.5.1)
project(mottrack)
add_definitions(-std=c++11)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Ofast -Wfatal-errors -D_MWAITXINTRIN_H_INCLUDED")

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

message("my_variable: ${OpenCV_INCLUDE_DIRS}")
include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(mottrack ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries( mottrack ${OpenCV_LIBS} )
target_link_libraries(mottrack ${PROJECT_SOURCE_DIR}/lib/libmottrack.so)


# install(TARGETS main DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
