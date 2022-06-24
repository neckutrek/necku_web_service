#include "utilities.h"
#include "io_context.h"

#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/json.hpp>

#ifndef TWELVEDATA_GENERIC_REST_REQUEST_H
#define TWELVEDATA_GENERIC_REST_REQUEST_H

namespace twelvedata {
namespace details {

class GenericRestRequest : public std::enable_shared_from_this<GenericRestRequest> {
private:
  twelvedata::detail::io_context ioc;
  boost::asio::ip::tcp::resolver resolver;
  boost::asio::ssl::context ctx;
  boost::beast::ssl_stream<boost::beast::tcp_stream> stream;
  boost::beast::http::request<boost::beast::http::string_body> http_request;
  boost::beast::flat_buffer read_buffer;
  boost::beast::http::response<boost::beast::http::dynamic_body> read_response;

  static constexpr auto endpoint_protocol = "https://";
  static constexpr auto endpoint_domain = "api.twelvedata.com";
  static constexpr auto endpoint_port = "443";

public:
  GenericRestRequest();

  boost::json::value
  send_request(const std::string &target);

  template<typename F>
  void
  async_send_request(
      const std::string &target,
      F &&completion_token);
};

template<typename F>
void
GenericRestRequest::async_send_request(
    const std::string &target,
    F &&completion_token)
{
  using namespace twelvedata::utilities;

  auto self{shared_from_this()};

  auto on_read = [self, completion_token](boost::system::error_code ec, size_t num_bytes) -> void {
    handle_error(ec, "Unable to read from tcp stream");
    boost::beast::get_lowest_layer(self->stream).close();

    std::stringstream ss{};
    ss << boost::beast::make_printable(self->read_response.body().data());
    std::string json_msg = ss.str();

    boost::json::parser parser;
    parser.write(json_msg, ec);
    handle_error(ec, "Unable to parser json");

    auto data = std::make_unique<boost::json::value>(parser.release());
    completion_token(std::move(data));
  };

  auto on_write = [self, on_read](boost::system::error_code ec, size_t num_bytes) -> void {
    handle_error(ec, "Unable to write to tcp stream");
    boost::beast::http::async_read(self->stream, self->read_buffer, self->read_response, on_read);
  };

  auto on_handshake = [self, target, on_write](boost::system::error_code ec) -> void {
    handle_error(ec, "Unable to ssl handshake");
    self->http_request.target(target);
    boost::beast::http::async_write(self->stream, self->http_request, on_write);
  };

  auto on_connect = [self, on_handshake](
      boost::system::error_code ec,
      const boost::asio::ip::tcp::endpoint &endpoint) -> void {
    handle_error(ec, "Unable to connect to socket");
    self->stream.async_handshake(boost::asio::ssl::stream_base::client, on_handshake);
  };

  auto on_resolve = [self, on_connect](
      boost::system::error_code ec,
      const boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> &endpoints) -> void {
    handle_error(ec, std::string{"Unable to resolve '"} + endpoint_domain + "'");
    boost::beast::get_lowest_layer(self->stream).async_connect(endpoints, on_connect);
  };

  resolver.async_resolve(endpoint_domain, endpoint_port, on_resolve);
}

}
}

#endif //TWELVEDATA_GENERIC_REST_REQUEST_H
