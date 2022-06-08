#include "trading_MarketChannelImpl.h"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include <boost/system/system_error.hpp>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>

//#include "root_certificates.hpp"

using namespace std::chrono_literals;

namespace websocket = boost::beast::websocket;
namespace ssl = boost::asio::ssl;

using tcp = boost::asio::ip::tcp;
using error_code = boost::system::error_code;
using ssl_stream = ssl::stream<tcp::socket>;
using ws_stream = websocket::stream<ssl_stream>;

void throw_if_failed(const error_code& ec, const std::string& msg)
{
  if (ec.failed())
  {
    std::string errmsg = msg + ". " + ec.message() + ". " + ec.category().name();
    throw boost::system::system_error(ec, errmsg);
  }
}

JNIEXPORT void JNICALL Java_trading_MarketChannelImpl_connect(
    JNIEnv *, jobject)
{
  std::cout << "Java_trading_MarketChannelImpl_connect" << std::endl;

  std::string host_domain{"ws.twelvedata.com"};
  std::string api_version{"/v1"};
  std::string route{"/quotes/price"};
  std::string api_key{"9a64360d06384744b56e618f1fc58b03"};

  std::string message{"{\"action\": \"heartbeat\"}"};

  try
  {
    boost::asio::io_context ioc{};
    error_code ec{};

    tcp::resolver resolver{ioc};
    const auto endpoints = resolver.resolve(host_domain, "443", ec);
    throw_if_failed(ec, "Failed to resolve domain name " + host_domain);

    ssl::context ctx{ssl::context::tlsv13};
    if (!SSL_CTX_set_options(ctx.native_handle(), SSL_OP_NO_TICKET))
    {
      error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
      throw_if_failed(ec, "Failed to set SSL_OP_NO_TICKET");
    }

    ws_stream ws_socket{ioc, ctx};
    if (!SSL_set_tlsext_host_name(ws_socket.next_layer().native_handle(), host_domain.c_str()))
    {
      error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
      throw_if_failed(ec, "Failed to set SNI hostname");
    }

    auto ep = boost::asio::connect(boost::beast::get_lowest_layer(ws_socket), endpoints, ec);
    throw_if_failed(ec, "Failed to connect to endpoints"); 
    std::cout << "Connected to: " << ep.address() << ":" << ep.port() << std::endl;

    ws_socket.set_option(websocket::stream_base::decorator(
          [&api_key](websocket::request_type& req){
              req.set("X-TD-APIKEY", api_key);
              std::cout << "Request: \n" << req << std::endl;
          }
      ));

    ws_socket.next_layer().handshake(ssl::stream_base::client, ec);
    throw_if_failed(ec, "Failed to SSL handshake");

    websocket::response_type res;
    ws_socket.handshake(res, host_domain, api_version+route, ec);
    throw_if_failed(ec, "Failed to handshake websocket");

    std::cout << "Response: " << res << std::endl;

    int n=0;
    while (n<3)
    {
      n++;
      ws_socket.write(boost::asio::buffer(message));
      std::cout << "Wrote: " << message << std::endl;
      boost::beast::multi_buffer buffer;
      ws_socket.read(buffer);
      std::cout << "Read from websocket" << std::endl;
      std::for_each(buffer.data().begin(), buffer.data().end(), [](auto el){
          std::cout.write(static_cast<const char*>(el.data()), el.size());
          });
      std::cout << std::endl;
      std::this_thread::sleep_for(100ms);
    }

    std::cout << "trying to close now" << std::endl;
    //ws_socket.next_layer().lowest_layer().close(ec);
    //throw_if_failed(ec, "Failed to close tcp connection");

    //int i; std::cout<<"Go now? "<<std::endl; std::cin>>i;
    ws_socket.close(websocket::close_code::normal, ec);
    throw_if_failed(ec, "Failed to close websocket");
  }
  catch (std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

