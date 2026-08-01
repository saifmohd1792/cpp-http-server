#include "socket.hpp"
#include "logger.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>

Socket::Socket()
    : fd_(-1)
{
}

Socket::Socket(int fd)
    : fd_(fd)
{
}

Socket::Socket(Socket&& other) noexcept
    : fd_(other.fd_)
{
    other.fd_ = -1;
}

Socket& Socket::operator=(Socket&& other) noexcept
{
    if (this != &other)
    {
        close();

        fd_ = other.fd_;

        other.fd_ = -1;
    }

    return *this;
}

Socket::~Socket()
{
    close();
}

bool Socket::create()
{
    fd_ = socket(AF_INET, SOCK_STREAM, 0);

    if (fd_ == -1)
    {
        Logger::error(
            std::string("socket(): ") +
            strerror(errno));

        return false;
    }

    return true;
}

bool Socket::set_reuse_addr()
{
    int opt = 1;

    if (setsockopt(
            fd_,
            SOL_SOCKET,
            SO_REUSEADDR,
            &opt,
            sizeof(opt)) == -1)
    {
        Logger::error(
            std::string("setsockopt(SO_REUSEADDR): ") +
            strerror(errno));

        return false;
    }

    return true;
}

bool Socket::set_non_blocking()
{
    int flags = fcntl(fd_, F_GETFL, 0);

    if (flags == -1)
    {
        Logger::error(
            std::string("fcntl(F_GETFL): ") +
            strerror(errno));

        return false;
    }

    if (fcntl(fd_, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        Logger::error(
            std::string("fcntl(F_SETFL): ") +
            strerror(errno));

        return false;
    }

    return true;
}

bool Socket::bind(int port)
{
    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (::bind(
            fd_,
            reinterpret_cast<sockaddr*>(&addr),
            sizeof(addr)) == -1)
    {
        Logger::error(
            std::string("bind(): ") +
            strerror(errno));

        return false;
    }

    return true;
}

bool Socket::listen(int backlog)
{
    if (::listen(fd_, backlog) == -1)
    {
        Logger::error(
            std::string("listen(): ") +
            strerror(errno));

        return false;
    }

    return true;
}

int Socket::accept()
{
    int client_fd = accept4(
        fd_,
        nullptr,
        nullptr,
        SOCK_NONBLOCK);

    if (client_fd == -1)
    {
        if (errno != EAGAIN &&
            errno != EWOULDBLOCK)
        {
            Logger::error(
                std::string("accept4(): ") +
                strerror(errno));
        }
    }

    return client_fd;
}

ssize_t Socket::receive(char* buffer,
                        std::size_t size)
{
    return recv(fd_, buffer, size, 0);
}

ssize_t Socket::send(const char* buffer,
                    std::size_t size)
{
    return ::send(fd_, buffer, size, 0);
}

void Socket::close()
{
    if (fd_ != -1)
    {
        ::close(fd_);

        fd_ = -1;
    }
}

int Socket::fd() const
{
    return fd_;
}

bool Socket::valid() const
{
    return fd_ != -1;
}