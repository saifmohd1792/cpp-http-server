#pragma once
#include <cstddef>
#include<sys/socket.h>
class Config 
{
    public:
    static constexpr int DEFAULT_PORT = 8080;
    static constexpr std::size_t BUFFER_SIZE=4096;
    static constexpr int MAX_EVENTS = 64;
    static constexpr int BACKLOG  = SOMAXCONN;
    static constexpr int EPOLL_TIMEOUT_MS = -1;
};