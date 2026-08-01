cmake_minimum_required(VERSION 3.16)

project(http_server)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_executable(
    http_server
    src/main.cpp
    src/connection.cpp
    src/http_parser.cpp
    src/http_router.cpp
    src/http_response.cpp
    src/handlers.cpp
    src/epoll_server.cpp
    src/config.cpp
    src/socket.cpp
    src/logger.cpp
    src/radix_tree.cpp
)

target_include_directories(
    http_server
    PRIVATE
    include
)