cmake_minimum_required(VERSION 3.18)

add_executable(server ../source/mserver.cpp ../source/server.cpp ../source/server.h ../source/code.h ../source/code.cpp)
add_executable(client ../source/mclient.cpp ../source/client.cpp ../source/client.h ../source/code.h ../source/code.cpp)
