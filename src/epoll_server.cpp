#include "epoll_server.hpp"

#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
EpollServer::EpollServer(int port)
    : port_(port),
      server_fd_(-1),
      epoll_fd_(-1)
{
}
void EpollServer::start()
{
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd_ == -1)
    {
        perror("socket");
        return;
    }

    std::cout << "Socket created\n";


int opt = 1;

setsockopt(
    server_fd_,
    SOL_SOCKET,
    SO_REUSEADDR,
    &opt,
    sizeof(opt));
sockaddr_in server_addr{};

server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(port_);
server_addr.sin_addr.s_addr = INADDR_ANY;

if (bind(
        server_fd_,
        reinterpret_cast<sockaddr*>(&server_addr),
        sizeof(server_addr)) == -1)
{
    perror("bind");
    close(server_fd_);
    return;
}
std::cout << "Bind successful\n";

if (listen(server_fd_, SOMAXCONN) == -1)
{
    perror("listen");
    close(server_fd_);
    return;
}

std::cout << "Listening on port "
          << port_
          << std::endl;
}
