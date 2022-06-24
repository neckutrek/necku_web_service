#ifndef TWELVEDATA_REST_REQUESTS_H
#define TWELVEDATA_REST_REQUESTS_H

#include "rest_request_types.h"

#include <memory>

namespace twelvedata {
namespace rest {

/**
 * @brief 
 * 
 * @return std::size_t 
 */
std::size_t
run_one();

/**
 *
 */
void
run_all();

/**
 * @brief Get the stocks list object
 * 
 * @param request 
 * @param apikey 
 * @return GetStocksListResponse 
 */
GetStocksListResponse
get_stocks_list(
    const GetStocksListRequest &request,
    const std::string &apikey);

/**
 * @brief 
 * 
 * @tparam F 
 * @param request 
 * @param apikey 
 * @param completion_token 
 */
template <typename F>
void
async_get_stocks_list(
    std::unique_ptr<GetStocksListRequest> request,
    const std::string& apikey,
    F&& completion_token);

}
}

#include "rest_requests.hpp"

#endif //TWELVEDATA_REST_REQUESTS_H
