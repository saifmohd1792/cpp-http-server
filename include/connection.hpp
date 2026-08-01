#pragma once

#include "socket.hpp"
#include "http_request.hpp"
#include <sys/socket.h>
#include <string>

class Connection
{
public:
    Connection() = default;

    explicit Connection(Socket socket);

    Socket& socket();

    const Socket& socket() const;

    HttpRequest& request();

    const HttpRequest& request() const;

    std::string& read_buffer();

    std::string& write_buffer();

    void clear();

private:

    Socket socket_;

    std::string read_buffer_;

    std::string write_buffer_;

    HttpRequest request_;

    bool keep_alive_ = false;
};