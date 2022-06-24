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

}
}

#endif //TWELVEDATA_UTILITIES_H
