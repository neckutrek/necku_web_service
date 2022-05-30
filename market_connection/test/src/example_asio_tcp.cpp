//#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>


#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/ip/resolver_base.hpp>
#include <boost/asio/ssl.hpp>

#include "utilities.h"

using tcp = boost::asio::ip::tcp;

TEST_CASE("example_asio")
{
  constexpr auto host = "ws.twelvedata.com";
  constexpr auto port = "80";

  SECTION("tcp", "[]")
  {
    try
    {
      boost::asio::io_context ioc{};
      auto resolver = tcp::resolver{ioc};
      auto endpoints = resolver.resolve(host, port);
      std::cout << "Resolved endpoints:" << std::endl;
      for (auto& ep : endpoints)
      {
        std::cout << ep.endpoint().address() << ":" << ep.endpoint().port() << std::endl;
      }

      boost::system::error_code ec{};

      tcp::socket sock{ioc};
      auto ep = boost::asio::connect(sock, endpoints, ec);
      REQUIRE_MESSAGE(!ec.failed(), "Failed to connect via tcp");
      std::cout << "Connected to: " << ep.address() << ":" << ep.port() << std::endl;

      sock.close(ec);
      REQUIRE_MESSAGE(!ec.failed(), "Failed to close tcp connection");
    }
    catch (std::exception& e)
    {
      WARN(e.what());
    }
  }

  SECTION("tcp_ssl", "[]")
  {
    try
    {
      boost::asio::io_context ioc{};
      auto resolver = tcp::resolver{ioc};
      auto endpoints = resolver.resolve(host, port);
      std::cout << "Resolved endpoints:" << std::endl;
      for (auto& ep : endpoints)
      {
        std::cout << ep.endpoint().address() << ":" << ep.endpoint().port() << std::endl;
      }

      boost::system::error_code ec{};

      boost::asio::ssl::context sslctx{boost::asio::ssl::context::tlsv13};
      boost::asio::ssl::stream<boost::asio::ip::tcp::socket> sock{ioc, sslctx};
      sock.lowest_layer().connect(*endpoints.begin(), ec);
      REQUIRE_MESSAGE(!ec.failed(), "Failed to connect via tcp/ssl");

      sock.lowest_layer().close(ec);
      REQUIRE_MESSAGE(!ec.failed(), "Failed to close tcp/ssl connection");
    }
    catch (std::exception& e)
    {
      WARN(e.what());
    }
  }

}

