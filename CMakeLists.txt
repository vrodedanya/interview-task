cmake_minimum_required(VERSION 3.18)

project(Server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -pthread -Wall")

add_executable(server ../source/mserver.cpp ../source/server.cpp ../source/server.h ../source/code.h ../source/code.cpp)
add_executable(client ../source/mclient.cpp ../source/client.cpp ../source/client.h ../source/code.h ../source/code.cpp)
