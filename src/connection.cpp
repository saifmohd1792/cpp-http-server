#include "connection.hpp"

Connection::Connection(Socket socket)
    : socket_(std::move(socket))
{
}

Socket& Connection::socket()
{
    return socket_;
}

const Socket& Connection::socket() const
{
    return socket_;
}

HttpRequest& Connection::request()
{
    return request_;
}

const HttpRequest& Connection::request() const
{
    return request_;
}

std::string& Connection::read_buffer()
{
    return read_buffer_;
}

std::string& Connection::write_buffer()
{
    return write_buffer_;
}

void Connection::clear()
{
    read_buffer_.clear();
    write_buffer_.clear();

    request_ = HttpRequest{};
}