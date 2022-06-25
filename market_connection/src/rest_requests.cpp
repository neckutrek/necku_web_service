#include "rest_requests.h"
#include "io_context.h"
#include "utilities.h"
#include "generic_rest_request.h"
#include "rest_request_types.h"

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
  return twelvedata::detail::native_ioc()->run_one();
}

void
run_all()
{
  while (run_one() != 0) {}
}

GetStocksListResponse
get_stocks_list(
    const GetStocksListRequest& request,
    const std::string& apikey)
{
  std::string target = fields_to_target_string("stocks", apikey, request);

  const auto json = send_request(target);

  GetStocksListResponse response{};
  from_json(json, response);
  return response;
}

}
}

