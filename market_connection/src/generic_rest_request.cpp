#include "generic_rest_request.h"

namespace twelvedata {
namespace details {

using namespace twelvedata::utilities;

GenericRestRequest::GenericRestRequest()
    : ioc{twelvedata::detail::native_ioc()}, resolver{*ioc}, ctx{boost::asio::ssl::context::tls}, stream{*ioc, ctx}
{
  http_request.method(boost::beast::http::verb::get);
  http_request.version(11);
  http_request.set(boost::beast::http::field::host, endpoint_domain);
  http_request.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
  http_request.set(boost::beast::http::field::accept, "*/*");

  boost::system::error_code ec;
  ctx.set_default_verify_paths(ec);
  handle_error(ec, "Unable to set default verify paths");
  ctx.set_verify_mode(boost::asio::ssl::verify_peer, ec);
  handle_error(ec, "Unable to set verify mode");

  if (!SSL_set_tlsext_host_name(stream.native_handle(), endpoint_domain)) {
    std::cerr << "Unable to set SNI hostname, " << ::ERR_get_error() << std::endl;
  }
}

boost::json::value
GenericRestRequest::send_request(const std::string &target)
{
  boost::system::error_code ec;

  const auto endpoints = resolver.resolve(endpoint_domain, endpoint_port, ec);
  handle_error(ec, std::string{"Unable to resolve '"} + endpoint_domain + "'");

  boost::beast::get_lowest_layer(stream).connect(endpoints, ec);
  handle_error(ec, "Unable to connect to socket");

  stream.handshake(boost::asio::ssl::stream_base::client, ec);
  handle_error(ec, "Unable to ssl handshake");

  //boost::beast::http::request<boost::beast::http::string_body> http_request{boost::beast::http::verb::get, target, 11};
  http_request.target(target);
  boost::beast::http::write(stream, http_request, ec);
  handle_error(ec, "Unable to write to tcp stream");

  boost::beast::http::read(stream, read_buffer, read_response, ec);
  handle_error(ec, "Unable to read from tcp stream");

  boost::beast::get_lowest_layer(stream).close();
  //stream.shutdown(ec);
  //if (ec == boost::beast::errc::not_connected || ec == boost::asio::error::eof)
  //{
  //    ec = {};
  //}
  //handle_error(ec, "Unable to shutdown tcp stream");

  std::stringstream ss{};
  ss << boost::beast::make_printable(read_response.body().data());
  std::string json_msg = ss.str();

  boost::json::parser parser;
  parser.write(json_msg, ec);
  handle_error(ec, "Unable to parser json");

  return parser.release();
}

}
}
