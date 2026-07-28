cmake_minimum_required(VERSION 3.16)

project(http_server)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    http_server
    src/main.cpp
    src/http_server.cpp
    src/http_parser.cpp
    src/http_router.cpp
    src/http_response.cpp
    src/handlers.cpp
)

target_include_directories(
    http_server
    PRIVATE
    include
)