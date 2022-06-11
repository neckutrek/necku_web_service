#include "trading_MarketChannelImpl.h"

#include <iostream>

#include "websocket_stream.h"

JNIEXPORT void JNICALL Java_trading_MarketChannelImpl_connect(
    JNIEnv *, jobject)
{
  std::cout << "Java_trading_MarketChannelImpl_connect" << std::endl;
  twelvedata::websocket::connect();
}
