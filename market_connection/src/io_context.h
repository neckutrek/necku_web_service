#ifndef TWELVEDATA_IO_CONTEXT_H
#define TWELVEDATA_IO_CONTEXT_H

#include <memory>

#include <boost/asio.hpp>

namespace twelvedata {
namespace detail {

using io_context = std::shared_ptr<boost::asio::io_context>;

io_context native_ioc();

}
}

#endif //TWELVEDATA_IO_CONTEXT_H
