#ifndef TWELVEDATA_REST_REQUEST_TYPES_H
#define TWELVEDATA_REST_REQUEST_TYPES_H

#include <string>
#include <optional>
#include <vector>
#include <array>
#include <utility>

#include <boost/json.hpp>

#include "utilities.h"

namespace twelvedata {
namespace rest {

namespace fields {

template<typename T, const char *Name>
struct RequestField {
  static constexpr const char *name = Name;
  std::optional<T> value;
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsubobject-linkage"

constexpr const char symbol[] = "symbol";
struct Symbol : RequestField<std::string, symbol> {};

constexpr char exchange[] = "exchange";
struct Exchange : RequestField<std::string, exchange> {};

constexpr char mic_code[] = "mic_code";
struct MicCode : RequestField<std::string, mic_code> {};

constexpr char country[] = "country";
struct Country : RequestField<std::string, country> {};

constexpr char name[] = "name";
struct Name : RequestField<std::string, name> {};

constexpr char type[] = "type";
struct Type : RequestField<std::string, type> {};

constexpr char currency[] = "currency";
struct Currency : RequestField<std::string, currency> {};

constexpr char format[] = "format";
struct Format : RequestField<std::string, format> {};

constexpr char delimiter[] = "delimiter";
struct Delimiter : RequestField<std::string, delimiter> {};

constexpr char show_plan[] = "show_plan";
struct ShowPlan : RequestField<bool, show_plan> {};

constexpr char include_delisted[] = "include_deslisted";
struct IncludeDelisted : RequestField<bool, include_delisted> {};

#pragma GCC diagnostic pop

} // namespace fields

///
using GetStocksListRequest = std::tuple<
    fields::Symbol,
    fields::Exchange,
    fields::MicCode,
    fields::Country,
    fields::Type,
    fields::Format,
    fields::Delimiter,
    fields::ShowPlan,
    fields::IncludeDelisted>;

template <typename... Fields>
struct ResponseBase
{
  using DataT = std::tuple<Fields...>;
  std::vector<DataT> data;
  std::string status;
};

struct GetStocksListResponse
: ResponseBase<
  fields::Symbol,
  fields::Name,
  fields::Currency,
  fields::Exchange,
  fields::MicCode,
  fields::Country,
  fields::Type> {};

/**
 *
 * @tparam P
 * @tparam Params
 * @param params
 * @return
 */
template <typename P, typename ...Params>
constexpr decltype(auto) get(Params&&... params)
{
  P* p = nullptr;
  constexpr auto try_set_p = [&p](auto& param){
    if constexpr (std::is_same<decltype(param), P>::value)
    {
      p = &param;
    }
  };
  utilities::for_each_argument(try_set_p, params...);
  return p;
}

template <typename... Params>
std::string
fields_to_target_string(
    const std::string& verb,
    const std::string& apikey,
    Params&&... params)
{
  std::string target = "/" + verb + "?apikey=" + apikey;
  auto add_field = [&target](auto& p) -> void {
    if (p.value) target += std::string{"&"} + p.name + "=" + std::string{p.value.value()};
  };
  std::apply(
      [&add_field](auto& ...x) -> void { utilities::for_each_argument(add_field, x...); },
      params...);
  return target;
}

template<typename ResponseT>
int from_json(const boost::json::value& json, ResponseT& response)
{
  auto status = boost::json::value_to<std::string>(json.at("status"));
  response.status = status;
  
  if (status.compare("ok") != 0)
  {
    return 1;
  }

  for (const auto &el: json.at("data").as_array()) {
    typename ResponseT::DataT data{};
    auto set_field = [&el](auto& field) -> void {
      using T = decltype(field.value.value());
      field.value = boost::json::value_to<typename std::remove_reference_t<T>>(el.at(field.name));
    };
    std::apply(
        [&set_field](auto&... fields) -> void { utilities::for_each_argument(set_field, fields...); },
        data);
    response.data.push_back(data);
  }
  return 0;
}

}
}

#endif //TWELVEDATA_REST_REQUEST_TYPES_H

