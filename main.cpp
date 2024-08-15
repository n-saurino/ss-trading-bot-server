#include <stdio.h>
#include <iostream>
#include <boost/version.hpp>

int main(int, char**){

   std::cout << "Boost version: " 
              << BOOST_VERSION / 100000 << "."  // Major version
              << BOOST_VERSION / 100 % 1000 << "."  // Minor version
              << BOOST_VERSION % 100  // Patch level
              << std::endl;
 

    int x = 42;
    printf("Hello, from main! x is %d.\n", x);
}
