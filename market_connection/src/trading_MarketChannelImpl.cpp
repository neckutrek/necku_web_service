#include "trading_MarketChannelImpl.h"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "root_certificates.hpp"

using namespace std::chrono_literals;

using tcp = boost::asio::ip::tcp;
namespace ip = boost::asio::ip;
namespace http = boost::beast::http;
namespace websocket = boost::beast::websocket;
namespace ssl = boost::asio::ssl;

JNIEXPORT void JNICALL Java_trading_MarketChannelImpl_connect(
    JNIEnv *, jobject)
{
  std::cout << "Java_trading_MarketChannelImpl_connect" << std::endl;

  std::string host_test{"echo.websocket.events"};

  std::string host_protocol{"wss://"};
  std::string host_domain{"ws.twelvedata.com"};
  std::string host_version{"/v1"};
  std::string host_route{"/quotes/price"};
  std::string host_port{"443"};

  std::string host_query{host_protocol + host_domain + host_version + host_route};

  std::string api_key{"9a64360d06384744b56e618f1fc58b03"};

  std::string message{"{\"action\": \"heartbeat\"}"};

  std::string dummy;

  try
  {
    boost::asio::io_context ioc;

    ssl::context ctx{ssl::context::tlsv13};
    load_root_certificates(ctx);
    std::cout << "Loaded root certificates" << std::endl;

    tcp::resolver resolver{ioc};
    const auto results = resolver.resolve(host_domain, host_port);
    std::cout << "Resolved host endpoints" << std::endl;

    websocket::stream<boost::beast::ssl_stream<tcp::socket>> ws{ioc, ctx};
    auto ep = boost::asio::connect(get_lowest_layer(ws), results.begin(), results.end());
    std::cout << "Connected (securely) to endpoint " << ep->endpoint().address() << ":" << ep->endpoint().port() << std::endl;
    host_port = std::to_string(ep->endpoint().port());

//   websocket::stream<tcp::socket> ws{ioc};
//   boost::asio::connect(ws.next_layer(), results.begin(), results.end());
//   std::cout << "Connected to endpoint " << ws.next_layer().remote_endpoint().address() << std::endl;

    if (!SSL_set_tlsext_host_name(ws.next_layer().native_handle(), host_domain.c_str()))
    {
      throw boost::beast::system_error(
        boost::beast::error_code(
            static_cast<int>(::ERR_get_error()),
            boost::asio::error::get_ssl_category()),
        "Failed to set SNI Hostname");
    }

    //using request_type = http::request<http::empty_body>;
    using request_type = websocket::request_type;
    ws.set_option(websocket::stream_base::decorator([&api_key](request_type& req){
        req.set("X-TD-APIKEY", api_key);
        std::cout << "Request: \n" << req << std::endl;
      }));
    std::cout << "Set decorator on Websocket HTTP upgrade request" << std::endl;

    std::cout << "Ready to handshake. Go on? ";
    std::cin >> dummy;

    websocket::response_type res;
    boost::system::error_code ec;
    //ws.handshake(res, host_domain+":"+host_port, "/", ec);
    ws.handshake(res, host_domain+":"+host_port, host_version+host_route, ec);
    if (!ec)
    {
      throw boost::system::system_error{ec};
    }
    std::cout << "ec: " << ec.message() << std::endl;
    std::cout << "Response: " << res << std::endl;
    std::cout << "Handshake done. Go on? (y/n) ";
    std::cin >> dummy;

    while (true)
    {
      ws.write(boost::asio::buffer(message));
      std::cout << "Wrote: " << message << std::endl;
      boost::beast::multi_buffer buffer;
      ws.read(buffer);
      std::cout << "Read from websocket" << std::endl;
      std::for_each(buffer.data().begin(), buffer.data().end(), [](auto el){
          std::cout.write(static_cast<const char*>(el.data()), el.size());
          });
      std::cout << std::endl;
      std::this_thread::sleep_for(1000ms);
    }

    ws.close(websocket::close_code::normal);
  }
  catch (std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

