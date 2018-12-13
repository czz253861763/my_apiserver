cmake_minimum_required(VERSION 3.13)
project(my_apiserver)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(my_apiserver
        Log.cpp
        Log.h
        MemPool.cpp
        MemPool.h
        server.cpp
        server.h
        main.cpp)
