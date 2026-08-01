#pragma once

#include <cstddef>
#include <sys/socket.h>

class Socket
{
public:
    Socket();
    explicit Socket(int fd);

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    ~Socket();

    bool create();

    bool bind(int port);

    bool listen(int backlog);

    int accept();

    ssize_t receive(char* buffer, std::size_t size);

    ssize_t send(const char* buffer, std::size_t size);

    bool set_non_blocking();

    bool set_reuse_addr();

    void close();

    int fd() const;

    bool valid() const;

private:
    int fd_;
};