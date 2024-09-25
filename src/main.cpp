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
#include "../include/Application.h"
#include "../include/Server.h"
#include "../include/Broker.h"
#include "../include/Sender.h"
#include <string>

// QuickFix
#include <quickfix/Session.h>
#include <quickfix/Application.h>
#include <quickfix/SessionSettings.h>
#include <quickfix/FileStore.h>
#include <quickfix/FileLog.h>
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
        /* Pass-through broker feed from Alpaca to Client
        net::io_context ioc;
        Broker broker(ioc);
        broker.Connect();
        broker.Authenticate();
        broker.Subscribe();
        */

        /* datetime server
        Server server;
        */
        std::string fileName = "../.vscode/FIX_Settings.json";

        FIX::SessionSettings settings(fileName);

        Application application;
        FIX::FileStoreFactory storeFactory(settings);
        FIX::FileLogFactory logFactory(settings);
        FIX::SocketAcceptor acceptor(application, storeFactory, settings, logFactory /*optional*/);
        acceptor.start();
        // while( condition == true ) { do something; }
        acceptor.stop();
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}