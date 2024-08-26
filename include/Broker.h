#include <stdio.h>
#include <iostream>
#include <fstream>
#include <boost/asio/ssl.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/version.hpp>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/json/src.hpp>  // To use Boost.JSON library
#include "../include/Server.h"
#include <string>

// QuickFIX
#include <quickfix/Application.h>
#include <quickfix/SessionSettings.h>
#include <quickfix/FileStore.h>
#include <quickfix/SocketInitiator.h>
#include <quickfix/Log.h>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = net::ip::tcp;
namespace json = boost::json;

class Broker
{
private:
    /* data */
    ssl::context ctx_;
    net::io_context& ioc_;
    websocket::stream<beast::ssl_stream<tcp::socket>> ws_;

public:
    Broker(boost::asio::io_context& ioc);
    ~Broker();
    void Connect();
    int Authenticate();
};

Broker::Broker(boost::asio::io_context& ioc) : ioc_(ioc), ctx_(ssl::context::tlsv12_client), ws_(ioc,ctx)
{
        ctx.set_default_verify_paths();
        websocket::stream<beast::ssl_stream<tcp::socket>> ws{ioc, ctx};
}

Broker::~Broker()
{
}

void Broker::Connect(){
    // Resolve the WebSocket server's domain name
    // auto const results = tcp::resolver{ioc}.resolve("paper-api.alpaca.markets", "443");
    auto const results = tcp::resolver{ioc_}.resolve("stream.data.alpaca.markets", "443");

    // Connect to the server
    net::connect(ws_.next_layer().next_layer(), results.begin(), results.end());

    // Set SNI Hostname (many hosts require this nowadays)
    // if(!SSL_set_tlsext_host_name(ws.next_layer().native_handle(), "paper-api.alpaca.markets")) {
    if(!SSL_set_tlsext_host_name(ws_.next_layer().native_handle(), "stream.data.alpaca.markets")) {
        beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
        throw beast::system_error{ec};
    }

    // Perform SSL handshake
    ws_.next_layer().handshake(ssl::stream_base::client);

    // Perform the WebSocket handshake
    ws_.handshake("stream.data.alpaca.markets", "/v2/test");
        // ws.handshake("paper-api.alpaca.markets", "/stream");
}

int Broker::Authenticate(){
    std::ifstream config_file("../.vscode/config.json");
    if (!config_file.is_open()) {
        std::cerr << "Could not open config file." << std::endl;
        return 1;
    }

    json::value config;
    config_file >> config;

    std::string api_key = config.as_object()["api_key"].as_string().c_str();
    std::string secret_key = config.as_object()["secret_key"].as_string().c_str();  
    // Authenticate with the API key and secret
    json::object auth_payload = {
        {"action", "auth"},
        {"key", api_key},
        {"secret", secret_key}
    };

    ws_.write(net::buffer(json::serialize(auth_payload)));
}