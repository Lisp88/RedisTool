cmake_minimum_required(VERSION 3.22)
project(RedisTool)

set(CMAKE_CXX_STANDARD 17)

add_executable(RedisTool main.cpp redis_tool.cpp redis_tool.h)
target_link_libraries(RedisTool /usr/local/lib/libhiredis.so)
