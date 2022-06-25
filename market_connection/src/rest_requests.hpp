#ifndef TWELVEDATA_REST_REQUESTS_HPP
#define TWELVEDATA_REST_REQUESTS_HPP

#include "generic_rest_request.h"
#include "io_context.h"
#include "utilities.h"
#include "rest_requests.h"

#include <boost/json.hpp>

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
  if (!request) {
    return;
  }

  std::string target = fields_to_target_string("stocks", apikey, *request);

  auto completion = [completion_token](std::unique_ptr<boost::json::value> json) -> void {
    auto response = std::make_unique<GetStocksListResponse>();
    from_json(*json, *response);
    completion_token(std::move(response));
    return;
  };

  async_send_request(target, completion);
}

}
}

#endif //TWELVEDATA_REST_REQUESTS_HPP
