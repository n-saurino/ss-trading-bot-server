#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>

constexpr short multicast_port = 30001;
constexpr int max_message_count = 10;


/* MULTICAST BROADCASTER OF MARKET/TRADE DATA */

class Sender
{
private:
    boost::asio::ip::udp::endpoint endpoint_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::steady_timer timer_;
    int message_count_;
    std::string message_;

    void send(){
        std::ostringstream oss;
        oss << "Message: " << message_count_++;

        message_ = oss.str();
        std::cout << message_ << std::endl;
        socket_.async_send_to(boost::asio::buffer(message_), endpoint_, 
        [this](boost::system::error_code ec, std::size_t /*lenght*/){ // lambda function!
            if (!ec && message_count_ < max_message_count){
                timeout();
            }
        });
    }

    void timeout(){
        timer_.expires_after(std::chrono::seconds(1));
        timer_.async_wait([this](boost::system::error_code ec){ // lambda function!
            if(!ec){
                send();
            }
        });
    }
    
public:
    Sender(boost::asio::io_context& ioc, const boost::asio::ip::address& multicast_address);
    ~Sender();
};

Sender::Sender(boost::asio::io_context& ioc, const boost::asio::ip::address& multicast_address) :
                endpoint_(multicast_address, multicast_port), socket_(ioc, endpoint_.protocol()), timer_(ioc), message_count_(0)
{
    send();
}

Sender::~Sender()
{
}
