#include "rest_requests.h"
#include "io_context.h"
#include "utilities.h"
#include "generic_rest_request.h"

#include <iostream>
#include <string>
#include <memory>
#include <chrono>
#include <thread>

#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/json.hpp>

using namespace std::chrono_literals;
using namespace twelvedata::utilities;

namespace {

boost::json::value
send_request(const std::string &target)
{
  twelvedata::details::GenericRestRequest obj{};
  return obj.send_request(target);
}

} // empty namespace

namespace twelvedata {
namespace rest {

std::size_t
run_one()
{
  return detail::native_ioc()->run_one();
}

void
run_all()
{
  while (run_one() != 0) {}
}

GetStocksListResponse
get_stocks_list(const GetStocksListRequest &request, const std::string &apikey) {
  std::string target = "/stocks?apikey=" + apikey;

  if (request.symbol) target += "&symbol=" + request.symbol.value();
  if (request.exchange) target += "&exchange=" + request.exchange.value();
  if (request.mic_code) target += "&mic_code=" + request.mic_code.value();
  if (request.country) target += "&country=" + request.country.value();
  if (request.type) target += "&type=" + request.type.value();
  if (request.format) target += "&format=" + request.format.value();
  if (request.delimiter) target += "&delimiter=" + request.delimiter.value();
  if (request.show_plan) target += "&show_plan=" + std::string{bool_to_cstr(request.show_plan.value())};
  if (request.include_delisted)
    target += "&include_delisted=" + std::string{bool_to_cstr(request.include_delisted.value())};

  const auto response = send_request(target);

  auto status = boost::json::value_to<std::string>(response.at("status"));
  if (status.compare("ok") != 0) {
    std::cerr << "Rest request status not 'ok', status=" << status << std::endl;
  }

  GetStocksListResponse result{};
  result.status = status;
  for (const auto &el: response.at("data").as_array()) {
    GetStocksListResponse::Data data{};
    data.symbol = boost::json::value_to<std::string>(el.at("symbol"));
    data.country = boost::json::value_to<std::string>(el.at("country"));
    data.currency = boost::json::value_to<std::string>(el.at("currency"));
    data.exchange = boost::json::value_to<std::string>(el.at("exchange"));
    data.mic_code = boost::json::value_to<std::string>(el.at("mic_code"));
    data.name = boost::json::value_to<std::string>(el.at("name"));
    data.type = boost::json::value_to<std::string>(el.at("type"));
    result.data.push_back(data);
  }
  return result;
}

}
}
