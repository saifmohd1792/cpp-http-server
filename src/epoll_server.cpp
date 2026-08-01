#include "epoll_server.hpp"
#include "http_parser.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "http_router.hpp"
#include "config.hpp"
#include  "socket.hpp"
#include "logger.hpp"
#include "connection.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <unordered_map>
EpollServer::EpollServer(int port)
    : port_(port),
      epoll_fd_(-1)
{
}

void EpollServer::start()
{
    if (!listener_.create())
        return;

    if (!listener_.set_reuse_addr())
        return;

    if (!listener_.set_non_blocking())
        return;

    if (!listener_.bind(Config::DEFAULT_PORT))
        return;

    if (!listener_.listen(Config::BACKLOG))
        return;

    epoll_fd_ = epoll_create1(0);

    if (epoll_fd_ == -1)
    {
        Logger::error("epoll_create1 failed");
        return;
    }

    epoll_event event{};
    event.events = EPOLLIN;
    event.data.fd = listener_.fd();

    if (epoll_ctl(
            epoll_fd_,
            EPOLL_CTL_ADD,
            listener_.fd(),
            &event) == -1)
    {
        Logger::error("epoll_ctl failed");
        return;
    }

    epoll_event events[Config::MAX_EVENTS];

    while (true)
    {
        int ready = epoll_wait(
            epoll_fd_,
            events,
            Config::MAX_EVENTS,
            Config::EPOLL_TIMEOUT_MS);

        if (ready == -1)
        {
            Logger::error("epoll_wait failed");
            continue;
        }

        for (int i = 0; i < ready; ++i)
        {
            if (events[i].data.fd == listener_.fd())
            {
                int client_fd = listener_.accept();

                if (client_fd == -1)
                    continue;

                Socket client(client_fd);

                connections_.emplace(
                    client_fd,
                    Connection(std::move(client)));

                epoll_event client_event{};
                client_event.events = EPOLLIN;
                client_event.data.fd = client_fd;

                epoll_ctl(
                    epoll_fd_,
                    EPOLL_CTL_ADD,
                    client_fd,
                    &client_event);
            }
            else
            {
                auto it = connections_.find(events[i].data.fd);

                if (it != connections_.end())
                    handle_client(it->second);
            }
        }
    }
}

   std::string EpollServer::receive_request(Connection& conn)
{
    char buffer[Config::BUFFER_SIZE];

    ssize_t bytes =
        conn.socket().receive(
            buffer,
            sizeof(buffer));

    if (bytes <= 0)
        return "";

    return std::string(buffer, bytes);
}
void EpollServer::send_response(
    Connection& conn,
    const HttpResponse& response)
{
    std::string data = response.to_string();

    conn.socket().send(
        data.data(),
        data.size());
}
void EpollServer::handle_client(Connection& conn)
{
    std::string raw_request =
        receive_request(conn);

    if (raw_request.empty())
    {
        int fd = conn.socket().fd();

        conn.socket().close();

        connections_.erase(fd);

        return;
    }

    HttpRequest request =
        HttpParser::parse(raw_request);

    HttpResponse response =
        router.route(request);

    send_response(conn, response);

    int fd = conn.socket().fd();

    conn.socket().close();

    connections_.erase(fd);
}
