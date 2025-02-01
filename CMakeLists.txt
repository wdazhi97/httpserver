cmake_minimum_required(VERSION 3.21)
project(httpserver)

set(CMAKE_CXX_STANDARD 20)

include_directories(third/boost_1_87_0)
include_directories(third/openssl/include)
include_directories(third/yaml/include)
link_directories(third/yaml/lib)
link_directories(third/openssl/lib)

add_executable(httpserver server.cpp connection.cpp connection.h httprequest.hpp httpresponse.hpp)

target_link_libraries(httpserver pthread ssl crypto yaml-cpp)
