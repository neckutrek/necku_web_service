#include "trading_MarketChannelImpl.h"

#include <iostream>
#include <string>

JNIEXPORT void JNICALL Java_trading_MarketChannelImpl_connect(
    JNIEnv *, jobject)
{
    std::cout << "Java_trading_MarketChannelImpl_connect" << std::endl;
}
