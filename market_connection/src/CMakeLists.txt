set(LIBNAME "market_connection")
set(HEADERS "include/trading_MarketChannelImpl.h")
set(SOURCES "trading_MarketChannelImpl.cpp")

add_library(${LIBNAME} SHARED ${SOURCES})
target_include_directories(${LIBNAME} PUBLIC include ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})
target_include_directories(${LIBNAME} PUBLIC include ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${LIBNAME} PUBLIC include ${BOOST_INCLUDE_DIRS})

set_target_properties(${LIBNAME} PROPERTIES PUBLIC_HEADER ${HEADERS})
install(TARGETS ${LIBNAME})

