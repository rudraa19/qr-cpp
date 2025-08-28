cmake_minimum_required(VERSION 3.10)
project(qr-cpp)

include_directories(include)

add_executable(qr_cpp
    src/qr.cpp
    src/main.cpp
)