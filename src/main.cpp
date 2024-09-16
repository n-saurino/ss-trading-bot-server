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
#include "../include/Broker.h"
#include "../include/Sender.h"
#include <string>

// QuickFix
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

int main() {
    
    try {
        /*
        net::io_context ioc;
        Broker broker(ioc);
        broker.Connect();
        broker.Authenticate();
        broker.Subscribe();
        */
       Server server;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}