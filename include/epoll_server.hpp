#pragma once

#include "connection.hpp"
#include "config.hpp"
#include "http_response.hpp"
#include "http_router.hpp"
#include "socket.hpp"

#include <sys/epoll.h>
#include <unordered_map>

class EpollServer
{
public:
    explicit EpollServer(int port);

    void start();

private:
    int port_;

    Socket listener_;

    int epoll_fd_;

    Router router;

    std::unordered_map<int, Connection> connections_;

    std::string receive_request(Connection& conn);

    void send_response(
        Connection& conn,
        const HttpResponse& response);

    void handle_client(Connection& conn);
};