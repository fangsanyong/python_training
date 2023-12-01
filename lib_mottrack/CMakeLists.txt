# CMake lowest version requirement
cmake_minimum_required(VERSION 3.5.1)
project(mottrack)
add_definitions(-std=c++11)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
# set(OpenCV_DIR  /home/fsy/fuck/opencv/lib/cmake/opencv4)
# include_directories(/usr/local/include/opencv4/opencv2)


# find_package(OpenCV)




# Compile options
# add_compile_options(
#     -std=c++11
#     -fPIC
#     -fstack-protector-all
#     -D_FORTIFY_SOURCE=2 -O2
#     )

# Specify target generation path
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "../out")
# set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -O0 -g -Wall")
# set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -O2 -Wall -s")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,relro,-z,now,-z,noexecstack -fPIE -pie")
# set(CMAKE_SKIP_RPATH TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Ofast -Wfatal-errors -D_MWAITXINTRIN_H_INCLUDED")



# FILE(GLOB SAMPLE_SRC_FILE *.cpp)
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

message("my_variable: ${OpenCV_INCLUDE_DIRS}")
include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/ DIR_SRC)

# add_executable(main ${DIR_SRC})
add_library(mottrack SHARED ${DIR_SRC})
target_link_libraries( mottrack ${OpenCV_LIBS} )


# install(TARGETS main DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
