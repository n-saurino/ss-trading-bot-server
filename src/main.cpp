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

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = net::ip::tcp;
namespace json = boost::json;

int main() {
    try {
        ssl::context ctx(ssl::context::tlsv12_client);
        ctx.set_default_verify_paths();
        net::io_context ioc;
        websocket::stream<beast::ssl_stream<tcp::socket>> ws{ioc, ctx};

        // Resolve the WebSocket server's domain name
        // auto const results = tcp::resolver{ioc}.resolve("paper-api.alpaca.markets", "443");
        auto const results = tcp::resolver{ioc}.resolve("stream.data.alpaca.markets", "443");

        // Connect to the server
        net::connect(ws.next_layer().next_layer(), results.begin(), results.end());

        // Set SNI Hostname (many hosts require this nowadays)
        // if(!SSL_set_tlsext_host_name(ws.next_layer().native_handle(), "paper-api.alpaca.markets")) {
        if(!SSL_set_tlsext_host_name(ws.next_layer().native_handle(), "stream.data.alpaca.markets")) {
            beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
            throw beast::system_error{ec};
        }

        // Perform SSL handshake
        ws.next_layer().handshake(ssl::stream_base::client);

        // Perform the WebSocket handshake
        ws.handshake("stream.data.alpaca.markets", "/v2/test");
         // ws.handshake("paper-api.alpaca.markets", "/stream");

         std::ifstream config_file("../.vscode/config.json");
        if (!config_file.is_open()) {
            std::cerr << "Could not open config file." << std::endl;
            return 1;
        }

        json::value config;
        config_file >> config;

        std::string api_key = config.as_object()["api_key"].as_string().c_str();
        std::string secret_key = config.as_object()["secret_key"].as_string().c_str();

        json::object auth_payload = {
            {"action", "auth"},
            {"key", api_key},
            {"secret", secret_key}
        };

        /*
        // Authenticate with the API key and secret
        json::object auth_payload = {
            {"action", "auth"},
            {"key", "PKHZO6X7CE2ZKC5ZI2ZY"},
            {"secret", "UcJwMeS0C3GTyJrP6OTDjSmOfIBXNsFDFefHVZLs"}
        };
        */
        ws.write(net::buffer(json::serialize(auth_payload)));

        // Buffer to hold incoming messages
        beast::flat_buffer buffer;

        // Subscribe to SPY trades
        json::object subscribe_payload = {
            {"action", "subscribe"},
            {"trades", {"FAKEPACA"}}
        };
        ws.write(net::buffer(json::serialize(subscribe_payload)));

        // Read messages in a loop and print them to stdout
        while (true) {
            buffer.consume(buffer.size());
            ws.read(buffer);
            std::cout << beast::make_printable(buffer.data()) << std::endl;
        }

        // Close the WebSocket connection
        ws.close(websocket::close_code::normal);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}