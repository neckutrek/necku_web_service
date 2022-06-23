#include "rest_requests.h"

#include <iostream>
#include <string>
#include <memory>
#include <chrono>
#include <thread>

#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/json.hpp>

using namespace std::chrono_literals;

namespace {
std::string endpoint_protocol = "https://";
std::string endpoint_domain = "api.twelvedata.com";
std::string endpoint_port = "443";

inline const char *
bool_to_cstr(const bool value) {
  return (value ? "true" : "false");
}

void
handle_error(const boost::system::error_code &ec, const std::string &msg) {
  if (ec.failed()) {
    std::cerr << msg << ". " << ec.message() << " (" << ec.category().name() << ")" << std::endl;
  }
}

class TDHttpClient : public std::enable_shared_from_this<TDHttpClient> {
private:
  boost::asio::io_context ioc;
  boost::asio::ip::tcp::resolver resolver;
  boost::asio::ssl::context ctx;
  boost::beast::ssl_stream<boost::beast::tcp_stream> stream;
  boost::beast::http::request<boost::beast::http::string_body> http_request;
  boost::beast::flat_buffer read_buffer;
  boost::beast::http::response<boost::beast::http::dynamic_body> read_response;

public:

  TDHttpClient()
  : ioc{}, resolver{ioc}, ctx{boost::asio::ssl::context::tls}, stream{ioc, ctx}
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

    if (!SSL_set_tlsext_host_name(stream.native_handle(), endpoint_domain.c_str())) {
      std::cerr << "Unable to set SNI hostname, " << ::ERR_get_error() << std::endl;
    }
  }

  boost::json::value
  make_request(const std::string &target)
  {
    boost::system::error_code ec;

    const auto endpoints = resolver.resolve(endpoint_domain, endpoint_port, ec);
    handle_error(ec, "Unable to resolve '" + endpoint_domain + "'");

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

  typedef void (*CompletionT)(std::unique_ptr<boost::json::value>);

  void
  async_make_request(
      const std::string& target,
      CompletionT completion_token)
  {
    auto on_read = [this, completion_token](boost::system::error_code ec, size_t num_bytes) -> void {
      handle_error(ec, "Unable to read from tcp stream");
      boost::beast::get_lowest_layer(stream).close();

      std::stringstream ss{};
      ss << boost::beast::make_printable(read_response.body().data());
      std::string json_msg = ss.str();

      boost::json::parser parser;
      parser.write(json_msg, ec);
      handle_error(ec, "Unable to parser json");

      completion_token(std::make_unique<boost::json::value>(parser.release()));
    };

    auto on_write = [this, on_read](boost::system::error_code ec, size_t num_bytes) -> void {
      handle_error(ec, "Unable to write to tcp stream");
      boost::beast::http::async_read(stream, read_buffer, read_response, on_read);
    };

    auto on_handshake = [this, target, on_write](boost::system::error_code ec) -> void {
      handle_error(ec, "Unable to ssl handshake");
      http_request.target(target);
      boost::beast::http::async_write(stream, http_request, on_write);
    };

    auto on_connect = [this, on_handshake](
        boost::system::error_code ec,
        const boost::asio::ip::tcp::endpoint& endpoint) -> void {
      std::cout << "on_connect" << std::endl;
      handle_error(ec, "Unable to connect to socket");
      stream.async_handshake(boost::asio::ssl::stream_base::client, on_handshake);
    };

    auto on_resolve = [this, on_connect](
        boost::system::error_code ec,
        const boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp>& endpoints) -> void {
      std::cout << "on_resolve" << std::endl;
      handle_error(ec, "Unable to resolve '" + endpoint_domain + "'");
      boost::beast::get_lowest_layer(stream).async_connect(endpoints, on_connect);
    };

    std::cout << "async_resolve" << std::endl;
    resolver.async_resolve(endpoint_domain, endpoint_port, on_resolve);
    ioc.run();
  }
};

boost::json::value
make_request(const std::string &target)
{
  TDHttpClient client{};
  return client.make_request(target);
}

void
async_make_request(
    const std::string &target,
    void(*completion_token)(std::unique_ptr<boost::json::value>))
{
  auto client = std::make_shared<TDHttpClient>();
  client->async_make_request(target, completion_token);
}

} // empty namespace

namespace twelvedata {
namespace rest {

std::atomic<int> val = 0;

void a_little_test(std::unique_ptr<boost::json::value> value)
{
  std::cout << "I'm here!" << std::endl;
  val = 1;
}

GetStocksListResponse
get_stocks_list(const GetStocksListRequest &request, const std::string &apikey) {
  std::string target = "/stocks?apikey=" + apikey;

  if (request.symbol) target += "&symbol=" + request.symbol.value();
  if (request.exchange) target += "&exchange=" + request.exchange.value();
  if (request.mic_code) target += "&mic_code=" + request.mic_code.value();
  if (request.country) target += "&country=" + request.country.value();
  if (request.type) target += "&type=" + request.type.value();
  if (request.format) target += "&format=" + request.format.value();
  if (request.delimiter) target += "&delimiter=" + request.delimiter.value();
  if (request.show_plan) target += "&show_plan=" + std::string{bool_to_cstr(request.show_plan.value())};
  if (request.include_delisted)
    target += "&include_delisted=" + std::string{bool_to_cstr(request.include_delisted.value())};

  const auto response = make_request(target);
  async_make_request(target, a_little_test);
  while (val == 0) {
    std::this_thread::sleep_for(100ms);
  }

  auto status = boost::json::value_to<std::string>(response.at("status"));
  if (status.compare("ok") != 0) {
    std::cerr << "Rest request status not 'ok', status=" << status << std::endl;
  }

  GetStocksListResponse result{};
  result.status = status;
  for (const auto &el: response.at("data").as_array()) {
    GetStocksListResponse::Data data{};
    data.symbol = boost::json::value_to<std::string>(el.at("symbol"));
    data.country = boost::json::value_to<std::string>(el.at("country"));
    data.currency = boost::json::value_to<std::string>(el.at("currency"));
    data.exchange = boost::json::value_to<std::string>(el.at("exchange"));
    data.mic_code = boost::json::value_to<std::string>(el.at("mic_code"));
    data.name = boost::json::value_to<std::string>(el.at("name"));
    data.type = boost::json::value_to<std::string>(el.at("type"));
    result.data.push_back(data);
  }
  return result;
}

void
async_get_stocks_list(
    std::unique_ptr<GetStocksListRequest> request,
    void(*completion_token)(std::unique_ptr<GetStocksListResponse>))
{

}

}
}
