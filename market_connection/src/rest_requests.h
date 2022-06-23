#ifndef TWELVEDATA_REST_REQUESTS_H
#define TWELVEDATA_REST_REQUESTS_H

#include "rest_request_types.h"

#include <memory>

namespace twelvedata {
namespace rest {

GetStocksListResponse get_stocks_list(const GetStocksListRequest &request, const std::string &apikey);
void async_get_stocks_list(std::unique_ptr<GetStocksListRequest> request, void(*completion_token)(std::unique_ptr<GetStocksListResponse>));

}
}

#endif //TWELVEDATA_REST_REQUESTS_H
