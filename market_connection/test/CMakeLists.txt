cmake_minimum_required(VERSION 3.15)
project(market_connection_tests CXX)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
find_package(market_connection CONFIG REQUIRED)
find_package(Catch2 REQUIRED)
find_package(OpenSSL REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

file(GLOB SOURCES "src/*.cpp")

add_executable(tests ${SOURCES})
target_link_libraries(tests Catch2::Catch2)
target_link_libraries(tests Threads::Threads)
target_link_libraries(tests OpenSSL::SSL)
target_link_libraries(tests OpenSSL::Crypto)

