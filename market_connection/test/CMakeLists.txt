cmake_minimum_required(VERSION 3.15)
project(market_connection_tests CXX)

find_package(market_connection CONFIG REQUIRED)
find_package(Catch2 REQUIRED)

add_executable(tests src/main.cpp)
target_link_libraries(tests Catch2::Catch2)

