#include "trading_MarketChannelImpl.h"

#include <iostream>
#include <vector>
#include <memory>

#include "websocket_stream.h"
#include "rest_request_types.h"
#include "rest_requests.h"

using namespace twelvedata::rest;

template <typename T, const char* Name>
std::ostream& operator<<(std::ostream& os, const twelvedata::rest::fields::RequestField<T, Name>& field)
{
  os << field.name << ": " << field.value.value() << std::endl;
  return os;
}

template <typename... Args>
std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& args)
{
  auto print = [&os](auto& arg){
    os << arg << ' ';
  };
  std::apply(
      [&print](auto&... as){ twelvedata::utilities::for_each_argument(print, as...); },
      args);
  return os;
}

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
  get<fields::Symbol>(req).value = "AAPL";
  auto response = get_stocks_list(req, apikey);
  std::cout << "sync: " << response.status << " : " << response.data.size() << std::endl;
  std::cout << response.data << std::endl;
}

void async_request(const std::string& apikey)
{
  auto req = std::make_unique<GetStocksListRequest>();
  get<fields::Symbol>(*req).value = "AAPL";
  auto completion = [](std::unique_ptr<GetStocksListResponse> response){
    std::cout << "async: " << response->status << " : " << response->data.size() << std::endl;
    std::cout << response->data << std::endl;
  };
  async_get_stocks_list(std::move(req), apikey, completion);
  run_all();
}

JNIEXPORT void JNICALL Java_trading_MarketChannelImpl_connect(
    JNIEnv *, jobject)
{
  std::cout << "Java_trading_MarketChannelImpl_connect" << std::endl;
  //twelvedata::websocket::connect();

  std::string apikey = "9a64360d06384744b56e618f1fc58b03";

  sync_request(apikey);
  std::cout << "- - - - -" << std::endl;
  async_request(apikey);
}
