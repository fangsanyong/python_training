cmake_minimum_required(VERSION 3.0.0)
project(resnet_demo VERSION 0.1.0 LANGUAGES C CXX)

set(MY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../")


message("当前路径： ${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB RTSP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/media/rtsp/src/*.c)
list(APPEND SRC ${RTSP_SRC})
file(GLOB APP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/app/*.c)
list(APPEND SRC ${APP_SRC})
file(GLOB OSD_SRC ${CMAKE_CURRENT_SOURCE_DIR}/media/osd/src/*.c)
list(APPEND SRC ${OSD_SRC})
file(GLOB AI_SRC ${CMAKE_CURRENT_SOURCE_DIR}/media/ai/src/*.cpp)
list(APPEND SRC ${AI_SRC})
file(GLOB AI_HELPER ${CMAKE_CURRENT_SOURCE_DIR}/media/ai/goturn/helper/*.cpp)
list(APPEND SRC ${AI_HELPER})
file(GLOB AI_LOADER ${CMAKE_CURRENT_SOURCE_DIR}/media/ai/goturn/loader/*.cpp)
list(APPEND SRC ${AI_LOADER})
file(GLOB AI_NETWORK ${CMAKE_CURRENT_SOURCE_DIR}/media/ai/goturn/network/*.cpp)
list(APPEND SRC ${AI_NETWORK})
file(GLOB AI_TRACKER ${CMAKE_CURRENT_SOURCE_DIR}/media/ai/goturn/tracker/*.cpp)
list(APPEND SRC ${AI_TRACKER})
file(GLOB AI_VIS ${CMAKE_CURRENT_SOURCE_DIR}/media/ai/goturn/visualizer/*.cpp)
list(APPEND SRC ${AI_VIS})
file(GLOB AI_NATIVE ${CMAKE_CURRENT_SOURCE_DIR}/media/ai/goturn/native/*.cpp)
list(APPEND SRC ${AI_NATIVE})
file(GLOB AI_CURENT ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(APPEND SRC ${AI_CURENT})
message("xxx： ${SRC}")
include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/media/onvif_v2.0/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/media/rtsp/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/media/osd/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/media/sdl/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/app/"
    "${CMAKE_CURRENT_SOURCE_DIR}/common/"
    "${MY_DIR}/SS928V100_SDK_V2.0.2.1/smp/a55_linux/mpp/out/include"
    )

add_executable(sample_onvif_rtsp ${SRC})
target_link_libraries(sample_onvif_rtsp stdc++ 
${CMAKE_CURRENT_SOURCE_DIR}/media/sdl/lib/libSDL_ttf.a
${CMAKE_CURRENT_SOURCE_DIR}/media/sdl/lib/libfreetype.a
${CMAKE_CURRENT_SOURCE_DIR}/media/sdl/lib/libSDL.a
)