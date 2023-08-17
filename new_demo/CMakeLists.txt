cmake_minimum_required(VERSION 3.0.0)
project(resnet_demo VERSION 0.1.0 LANGUAGES C CXX)
message("当前路径： ${CMAKE_CURRENT_SOURCE_DIR}")

aux_source_directory(./src SRC)
include_directories(
    "./inc"
    "./apendens/opencv-4.5.2/include/opencv4"
    "./apendens/Ascend/ascend-toolkit/5.10.t20.0.b200/arm64-lmixlinux200/aarch64-linux/include"
    )
add_executable(resnet_demo ${SRC})

target_link_libraries(resnet_demo stdc++ 
${CMAKE_CURRENT_SOURCE_DIR}/apendens/Ascend/ascend-toolkit/5.10.t20.0.b200/arm64-lmixlinux200/aarch64-linux/lib64/libascendcl.so
${CMAKE_CURRENT_SOURCE_DIR}/apendens/Ascend/ascend-toolkit/5.10.t20.0.b200/arm64-lmixlinux200/aarch64-linux/lib64/libmsprofiler.so
${CMAKE_CURRENT_SOURCE_DIR}/apendens/Ascend/ascend-toolkit/5.10.t20.0.b200/arm64-lmixlinux200/aarch64-linux/lib64/libge_executor.so
${CMAKE_CURRENT_SOURCE_DIR}/apendens/Ascend/ascend-toolkit/5.10.t20.0.b200/arm64-lmixlinux200/aarch64-linux/lib64/libruntime.so
${CMAKE_CURRENT_SOURCE_DIR}/apendens/Ascend/ascend-toolkit/5.10.t20.0.b200/arm64-lmixlinux200/aarch64-linux/lib64/libalog.so
${CMAKE_CURRENT_SOURCE_DIR}/apendens/Ascend/ascend-toolkit/5.10.t20.0.b200/arm64-lmixlinux200/aarch64-linux/lib64/libascend_protobuf.so.3.13.0.0
${CMAKE_CURRENT_SOURCE_DIR}/apendens/Ascend/ascend-toolkit/5.10.t20.0.b200/arm64-lmixlinux200/aarch64-linux/lib64/libregister.so

${CMAKE_CURRENT_SOURCE_DIR}/apendens/aarch64-mix210-linux/aarch64-linux-gnu/lib64/libgomp.so.1

${CMAKE_CURRENT_SOURCE_DIR}/apendens/opencv-4.5.2/lib/libopencv_highgui.so
${CMAKE_CURRENT_SOURCE_DIR}/apendens/opencv-4.5.2/lib/libopencv_imgcodecs.so
${CMAKE_CURRENT_SOURCE_DIR}/apendens/opencv-4.5.2/lib/libopencv_core.so
${CMAKE_CURRENT_SOURCE_DIR}/apendens/opencv-4.5.2/lib/libopencv_imgproc.so 

${CMAKE_CURRENT_SOURCE_DIR}/apendens/libmpp021/lib/npu/libmmpa.so
${CMAKE_CURRENT_SOURCE_DIR}/apendens/libmpp021/lib/npu/libmmpa.a
${CMAKE_CURRENT_SOURCE_DIR}/apendens/libmpp021/lib/npu/libdrvdevdrv.so
${CMAKE_CURRENT_SOURCE_DIR}/apendens/libmpp021/lib/npu/libdrv_dfx.so
${CMAKE_CURRENT_SOURCE_DIR}/apendens/libmpp021/lib/npu/libge_common.so
${CMAKE_CURRENT_SOURCE_DIR}/apendens/libmpp021/lib/npu/libgraph.so
${CMAKE_CURRENT_SOURCE_DIR}/apendens/libmpp021/lib/npu/liberror_manager.so
${CMAKE_CURRENT_SOURCE_DIR}/apendens/libmpp021/lib/npu/libc_sec.so
${CMAKE_CURRENT_SOURCE_DIR}/apendens/libmpp021/lib/npu/libslog.so
)  
