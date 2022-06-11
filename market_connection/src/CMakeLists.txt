set(LIBNAME "market_connection")
set(HEADERS include/trading_MarketChannelImpl.h websocket_stream.h)
set(SOURCES trading_MarketChannelImpl.cpp websocket_stream.cpp)

add_library(${LIBNAME} SHARED ${SOURCES})

target_include_directories(${LIBNAME} PUBLIC include ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})
target_include_directories(${LIBNAME} PUBLIC include ${PROJECT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${LIBNAME} PUBLIC include ${BOOST_INCLUDE_DIRS})
target_include_directories(${LIBNAME} PUBLIC include ${OPENSSL_INCLUDE_DIR})

target_link_libraries(${LIBNAME} ${OPENSSL_LIBRARIES})

set_target_properties(${LIBNAME} PROPERTIES PUBLIC_HEADER "${PROJECT_SOURCE_DIR}/${HEADERS}")
install(
  TARGETS ${LIBNAME}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

