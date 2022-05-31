//#define CATCH_CONFIG_RUNNER
#include <boost/asio/ssl/verify_mode.hpp>
#include <catch2/catch.hpp>


#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/ip/resolver_base.hpp>
#include <boost/asio/ssl.hpp>

#include "utilities.h"

namespace ssl = boost::asio::ssl;

using tcp         = boost::asio::ip::tcp;
using ssl_socket  = ssl::stream<tcp::socket>;
using error_code  = boost::system::error_code;

void test_ec(const error_code& ec, const std::string& msg)
{
  REQUIRE_MESSAGE(!ec.failed(), msg + ". " + ec.message() + ", " + ec.category().name());
}

TEST_CASE("example_asio")
{
  constexpr auto td_host = "ws.twelvedata.com";
  constexpr auto echo_host = "echo.websocket.events";

  constexpr auto cert_file = "/git/necku_web_service/certs/cacert.pem";

  SECTION("tcp", "[]")
  {
    std::cout << "http://" << echo_host << std::endl;
    try
    {
      boost::asio::io_context ioc{};
      auto resolver = tcp::resolver{ioc};
      auto endpoints = resolver.resolve(echo_host, "80");
      std::cout << "Resolved endpoints:" << std::endl;
      for (auto& ep : endpoints)
      {
        std::cout << ep.endpoint().address() << ":" << ep.endpoint().port() << std::endl;
      }

      error_code ec{};

      tcp::socket sock{ioc};
      auto ep = boost::asio::connect(sock, endpoints, ec);
      test_ec(ec, "Failed to connect via tcp");
      std::cout << "Connected to: " << ep.address() << ":" << ep.port() << std::endl;

      sock.close(ec);
      test_ec(ec, "Failed to close tcp connection");
    }
    catch (std::exception& e)
    {
      WARN(e.what());
    }
    std::cout << std::endl;
  }

  SECTION("google_ssl", "[]")
  {
    std::cout << "https://google.com" << std::endl;

    boost::asio::io_context io_context;
    ssl::context ssl_context(ssl::context::tls);
    ssl_socket socket(io_context, ssl_context);
    tcp::resolver resolver(io_context);
    error_code ec{};

    auto endpoints = resolver.resolve("google.com", "443", ec);
    test_ec(ec, "Failed to resolve URI google.com:443");
    for (auto& ep : endpoints)
    {
      std::cout << ep.endpoint().address() << ":" << ep.endpoint().port() << std::endl;
    }

    boost::asio::connect(socket.next_layer(), endpoints, ec);
    test_ec(ec, "Failed to connect tcp");

    socket.handshake(ssl::stream_base::client, ec);
    test_ec(ec, "Failed to handshake ssl");

    socket.next_layer().close(ec);
    test_ec(ec, "Failed to close tcp connection");
  }

  SECTION("tcp_ssl", "[]")
  {
    std::cout << "https://" << echo_host << std::endl;
    try
    {
      boost::asio::io_context ioc{};
      error_code ec{};

      auto resolver = tcp::resolver{ioc};
      auto endpoints = resolver.resolve(echo_host, "443", ec);
      std::cout << "Resolved endpoints:" << std::endl;
      for (auto& ep : endpoints)
      {
        std::cout << ep.endpoint().address() << ":" << ep.endpoint().port() << std::endl;
      }

      ssl::context ctx{ssl::context::tls};
      ssl_socket sock{ioc, ctx};

      auto& ep = *endpoints.begin();
      boost::asio::connect(sock.lowest_layer(), endpoints, ec);
      test_ec(ec, "Failed to connect via tcp/ssl");
      std::cout << "Connected to: " << ep.endpoint().address() << ":" << ep.endpoint().port() << std::endl;
      
      sock.handshake(ssl::stream_base::client, ec);
      test_ec(ec, "Failed to handshake over ssl as client");

      sock.lowest_layer().close(ec);
      test_ec(ec, "Failed to close tcp/ssl connection");
    }
    catch (std::exception& e)
    {
      WARN(e.what());
    }
    std::cout << std::endl;
  }

  SECTION("tcp_td", "[]")
  {
    std::cout << "http://" << td_host << std::endl;
    try
    {
      boost::asio::io_context ioc{};
      auto resolver = tcp::resolver{ioc};
      auto endpoints = resolver.resolve(td_host, "80");
      std::cout << "Resolved endpoints:" << std::endl;
      for (auto& ep : endpoints)
      {
        std::cout << ep.endpoint().address() << ":" << ep.endpoint().port() << std::endl;
      }

      error_code ec{};

      tcp::socket sock{ioc};
      auto ep = boost::asio::connect(sock, endpoints, ec);
      test_ec(ec, "Failed to connect via tcp");
      std::cout << "Connected to: " << ep.address() << ":" << ep.port() << std::endl;

      sock.close(ec);
      test_ec(ec, "Failed to close tcp connection");
    }
    catch (std::exception& e)
    {
      WARN(e.what());
    }
    std::cout << std::endl;
  }

  SECTION("tcp_ssl_td", "[]")
  {
    std::cout << "https://" << td_host << std::endl;
    try
    {
      boost::asio::io_context ioc{};
      auto resolver = tcp::resolver{ioc};
      auto endpoints = resolver.resolve(td_host, "443");
      std::cout << "Resolved endpoints:" << std::endl;
      for (auto& ep : endpoints)
      {
        std::cout << ep.endpoint().address() << ":" << ep.endpoint().port() << std::endl;
      }

      error_code ec{};

      ssl::context sslctx{ssl::context::tlsv13};
      ssl::stream<tcp::socket> sock{ioc, sslctx};

      if (!SSL_set_tlsext_host_name(sock.native_handle(), td_host))
      {
        error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
        test_ec(ec, "Failed to set ssl host name");
      }

      auto& ep = *endpoints.begin();
      boost::asio::connect(sock.lowest_layer(), endpoints, ec);
      test_ec(ec, "Failed to connect via tcp/ssl");
      std::cout << "Connected to: " << ep.endpoint().address() << ":" << ep.endpoint().port() << std::endl;

      sock.handshake(ssl::stream_base::client, ec);
      //sock.handshake(ssl::stream<tcp::socket>::client, ec);
      test_ec(ec, "Failed to handshake over ssl");

      sock.lowest_layer().close(ec);
      test_ec(ec, "Failed to close tcp/ssl connection");
    }
    catch (std::exception& e)
    {
      WARN(e.what());
    }
    std::cout << std::endl;
  }

}

