#include "trading_MarketChannelImpl.h"

#include <iostream>
#include <vector>
#include <memory>

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


void sync_request(const std::string& apikey)
{
  GetStocksListRequest req;
  req.symbol = "AAPL";
  auto response = get_stocks_list(req, apikey);
  std::cout << response.status << std::endl;
}

void async_request(const std::string& apikey)
{
  auto req2 = std::make_unique<GetStocksListRequest>();
  req2->symbol = "AAPL";
  auto completion = [](std::unique_ptr<GetStocksListResponse> response){
    std::cout << response->status << " : " << response->data.size() << std::endl;
  };
  async_get_stocks_list(std::move(req2), apikey, completion);
  run_all();
}

JNIEXPORT void JNICALL Java_trading_MarketChannelImpl_connect(
    JNIEnv *, jobject)
{
  std::cout << "Java_trading_MarketChannelImpl_connect" << std::endl;
  //twelvedata::websocket::connect();

  std::string apikey = "9a64360d06384744b56e618f1fc58b03";

  //sync_request(apikey);
  std::cout << "- - - - -" << std::endl;
  async_request(apikey);
}
