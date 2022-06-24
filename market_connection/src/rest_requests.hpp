#include "generic_rest_request.h"
#include "io_context.h"
#include "utilities.h"

#include <boost/json.hpp>

#ifndef TWELVEDATA_REST_REQUESTS_HPP
#define TWELVEDATA_REST_REQUESTS_HPP

namespace {

template <typename F>
void
async_send_request(
    const std::string &target,
    F&& completion_token)
{
  auto obj = std::make_shared<twelvedata::details::GenericRestRequest>();
  obj->async_send_request(target, completion_token);
}

}

namespace twelvedata {
namespace rest {

template<typename F>
void
async_get_stocks_list(
    std::unique_ptr<GetStocksListRequest> request,
    const std::string &apikey,
    F &&completion_token)
{
  using namespace twelvedata::utilities;

  if (!request) {
    return;
  }

  std::string target = "/stocks?apikey=" + apikey;

  if (request->symbol) target += "&symbol=" + request->symbol.value();
  if (request->exchange) target += "&exchange=" + request->exchange.value();
  if (request->mic_code) target += "&mic_code=" + request->mic_code.value();
  if (request->country) target += "&country=" + request->country.value();
  if (request->type) target += "&type=" + request->type.value();
  if (request->format) target += "&format=" + request->format.value();
  if (request->delimiter) target += "&delimiter=" + request->delimiter.value();
  if (request->show_plan) target += "&show_plan=" + std::string{bool_to_cstr(request->show_plan.value())};
  if (request->include_delisted)
    target += "&include_delisted=" + std::string{bool_to_cstr(request->include_delisted.value())};

  auto completion = [completion_token](std::unique_ptr<boost::json::value> response) -> void {
    auto status = boost::json::value_to<std::string>(response->at("status"));
    if (status.compare("ok") != 0) {
      std::cerr << "Rest request status not 'ok', status=" << status << std::endl;
    }

    auto result = std::make_unique<GetStocksListResponse>();
    result->status = status;
    for (const auto &el: response->at("data").as_array()) {
      GetStocksListResponse::Data data{};
      data.symbol = boost::json::value_to<std::string>(el.at("symbol"));
      data.country = boost::json::value_to<std::string>(el.at("country"));
      data.currency = boost::json::value_to<std::string>(el.at("currency"));
      data.exchange = boost::json::value_to<std::string>(el.at("exchange"));
      data.mic_code = boost::json::value_to<std::string>(el.at("mic_code"));
      data.name = boost::json::value_to<std::string>(el.at("name"));
      data.type = boost::json::value_to<std::string>(el.at("type"));
      result->data.push_back(data);
    }

    completion_token(std::move(result));
  };

  async_send_request(target, completion);
}

}
}

#endif //TWELVEDATA_REST_REQUESTS_HPP
