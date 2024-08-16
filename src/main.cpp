#include <stdio.h>
#include <iostream>
#include <boost/version.hpp>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/json/src.hpp>  // To use Boost.JSON library
#include "../include/Server.h"

int main(int, char**){
    // Server server;

    std::cout << "Boost version: " 
              << BOOST_VERSION / 100000 << "."  // Major version
              << BOOST_VERSION / 100 % 1000 << "."  // Minor version
              << BOOST_VERSION % 100  // Patch level
              << std::endl;
    

    return 0;
}

