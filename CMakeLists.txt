cmake_minimum_required(VERSION 3.23)
project(SimpleXor)

#set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -pthread")

add_executable(SimpleXor main.cpp)
