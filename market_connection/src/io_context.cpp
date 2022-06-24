#include "io_context.h"

namespace {

std::shared_ptr<boost::asio::io_context> io_context;

}

namespace twelvedata {
namespace detail {

io_context
native_ioc()
{
  if (!::io_context)
  {
    ::io_context.reset(new boost::asio::io_context{});
  }
  return ::io_context;
}

}
}
