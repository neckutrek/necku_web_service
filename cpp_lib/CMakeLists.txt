cmake_minimum_required(VERSION 3.15)
project(native CXX)

find_package(JNI 1.8 REQUIRED)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

add_library(native SHARED src/demo_NativeInterface.cpp)
target_include_directories(native PUBLIC include ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})

set_target_properties(native PROPERTIES PUBLIC_HEADER "include/demo_NativeInterface.h")
install(TARGETS native)
