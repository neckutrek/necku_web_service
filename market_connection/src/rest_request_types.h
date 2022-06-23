#ifndef TWELVEDATA_REST_REQUEST_TYPES_H
#define TWELVEDATA_REST_REQUEST_TYPES_H

#include <string>
#include <optional>
#include <vector>
#include <array>
#include <utility>

#include <boost/optional.hpp>
#include <boost/variant.hpp>

template<size_t NParams>
struct ParameterBundle
{
  using ParamValueType = boost::variant<std::string, bool>;
  using ParametersT = std::array<std::pair<std::string, boost::optional<ParamValueType>>, NParams>;

  ParametersT make_parameters(const std::array<std::string, NParams>& parameter_names)
  {
    return {};
  }
};

namespace twelvedata {
namespace rest {

struct GetStocksListRequest {
  std::optional<std::string> symbol;
  std::optional<std::string> exchange;
  std::optional<std::string> mic_code;
  std::optional<std::string> country;
  std::optional<std::string> type;
  std::optional<std::string> format;
  std::optional<std::string> delimiter;
  std::optional<bool> show_plan;
  std::optional<bool> include_delisted;
};

struct GetStocksListResponse {
  struct Data {
    std::string symbol;
    std::string name;
    std::string currency;
    std::string exchange;
    std::string mic_code;
    std::string country;
    std::string type;
  };

  std::string status;
  std::vector<Data> data;
};

}
}

#endif //TWELVEDATA_REST_REQUEST_TYPES_H
