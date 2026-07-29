#pragma once

#include <sys/epoll.h>

class EpollServer
{
public:
    explicit EpollServer(int port);

    void start();

private:
    int port_;

    int server_fd_;

    int epoll_fd_;
};