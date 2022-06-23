#include "trading_MarketChannelImpl.h"

#include <iostream>
#include <vector>

#include "websocket_stream.h"
#include "rest_request_types.h"
#include "rest_requests.h"

using namespace twelvedata::rest;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
  for (const auto& el : vec)
  {
    os << el << ' ';
  }
  return os;
}

JNIEXPORT void JNICALL Java_trading_MarketChannelImpl_connect(
    JNIEnv *, jobject)
{
  std::cout << "Java_trading_MarketChannelImpl_connect" << std::endl;
  //twelvedata::websocket::connect();

  GetStocksListRequest req;
  req.symbol = "AAPL";
  auto response = get_stocks_list(req, "9a64360d06384744b56e618f1fc58b03");
  std::cout << response.status << std::endl;
}
