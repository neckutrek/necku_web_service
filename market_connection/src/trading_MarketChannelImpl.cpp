#include "trading_MarketChannelImpl.h"

#include <iostream>
#include <string>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;

JNIEXPORT void JNICALL Java_trading_MarketChannelImpl_connect(
    JNIEnv *, jobject)
{
  std::cout << "Java_trading_MarketChannelImpl_connect" << std::endl;
  std::string host{"echo.websocket.events"};
  std::string port{"80"};
  std::string text{""};

  try
  {
    boost::asio::io_context ioc;
    tcp::resolver resolver{ioc};
    websocket::stream<tcp::socket> ws{ioc};
    const auto results = resolver.resolve(host, port);
    boost::asio::connect(ws.next_layer(), results.begin(), results.end());
    ws.handshake(host, "/");
    ws.write(boost::asio::buffer(text));
    boost::beast::multi_buffer buffer;
    ws.read(buffer);
    ws.close(websocket::close_code::normal);

    std::for_each(buffer.data().begin(), buffer.data().end(), [](auto el){
        std::cout.write(static_cast<const char*>(el.data()), el.size());
        });
    std::cout << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

