#ifndef TWELVEDATA_UTILITIES_H
#define TWELVEDATA_UTILITIES_H

#include <string>
#include <iostream>

#include <boost/system/error_code.hpp>

namespace twelvedata {
namespace utilities {

inline const char *
bool_to_cstr(const bool value)
{
  return (value ? "true" : "false");
}

inline void
handle_error(const boost::system::error_code &ec, const std::string &msg)
{
  if (ec.failed()) {
    std::cerr << msg << ". " << ec.message() << " (" << ec.category().name() << ")" << std::endl;
  }
}

template <class F, class... Args>
constexpr void for_each_argument(F&& f, Args&&... args)
{
  [](...){}((f(std::forward<Args>(args)), 0)...);
}

constexpr bool streq(const char* s1, const char* s2)
{
  return std::string_view(s1) == s2;
};

}
}

#endif //TWELVEDATA_UTILITIES_H
