#pragma once
#include <boost/asio.hpp>
#include <ctime>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

class Server
{
private:
    /* data */
    std::string make_date_string();
    int Start();

public:
    Server(/* args */);
    ~Server();
};

Server::Server(/* args */)
{
    Start();   
}

Server::~Server()
{
}

std::string Server::make_date_string(){
    using namespace std;
    time_t now = time(0); 
    return ctime(&now);
}

int Server::Start(){
    try
    {
        boost::asio::io_context io;
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 13));
        while(true){
            // This is an iterative server, which means that it will handle one connection at a time.
            // Create a socket that will represent the connection to the client, 
            // and then wait for a connection.
            tcp::socket socket(io);
            acceptor.accept(socket);

            // A client is accessing our service. Determine the current time and 
            // transfer this information to the client.
            std::string message = make_date_string();
            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}



