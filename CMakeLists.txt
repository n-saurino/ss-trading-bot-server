cmake_minimum_required(VERSION 3.5.0)
project(main VERSION 0.1.0 LANGUAGES C CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)  # Use C++17, you can change this to 11, 14, 20, etc.
set(CMAKE_CXX_STANDARD_REQUIRED True)  # Ensure the compiler enforces the standard
set(CMAKE_CXX_EXTENSIONS OFF)  # Disable compiler-specific extensions

# Set Ninja as the default generator if available
if(NOT CMAKE_GENERATOR AND "${CMAKE_GENERATOR}" STREQUAL "")
    if(CMAKE_MAKE_PROGRAM MATCHES "ninja")
        set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "" FORCE)
    endif()
endif()

# Find Boost
# set(BOOST_ROOT "/opt/homebrew/Cellar/boost/1.85.0")
# find_package(Boost 1.85.0 REQUIRED COMPONENTS system filesystem)
# Require at least Boost 1.71.0, but allow newer versions
find_package(Boost 1.71.0 REQUIRED)
find_package(OpenSSL REQUIRED)

# Include QuickFIX
find_library(QUICKFIX_LIB quickfix)
include_directories(/usr/local/include/quickfix)

# Verify QuickFIX library
if(NOT QUICKFIX_LIB)
    message(FATAL_ERROR "Could not find QuickFIX library")
endif()

if(Boost_FOUND)
    message(STATUS "Boost version: ${Boost_VERSION}")
    message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost library dirs: ${Boost_LIBRARY_DIRS}")
else()
    message(FATAL_ERROR "Could not find Boost")
endif()

add_executable(main src/main.cpp include/Server.h)

# Include directories
target_include_directories(main PRIVATE ${Boost_INCLUDE_DIRS} /usr/local/include/quickfix)

# Link libraries
target_link_libraries(main PRIVATE ${Boost_LIBRARIES} ${QUICKFIX_LIB} pthread OpenSSL::SSL OpenSSL::Crypto)
