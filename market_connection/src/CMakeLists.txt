set(LIBNAME "market_connection")
set(SOURCES
    trading_MarketChannelImpl.cpp
    websocket_stream.cpp
    rest_requests.cpp
    io_context.cpp
    generic_rest_request.cpp)

add_library(${LIBNAME} SHARED ${SOURCES})

target_include_directories(${LIBNAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(${LIBNAME} PUBLIC ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})
target_include_directories(${LIBNAME} PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(${LIBNAME} OpenSSL::SSL)
target_link_libraries(${LIBNAME} OpenSSL::Crypto)
target_link_libraries(${LIBNAME} Boost::json)

install(
  TARGETS ${LIBNAME}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

