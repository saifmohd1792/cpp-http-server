#include "http_server.hpp"

#include "http_request.hpp"
#include "http_response.hpp"
#include "http_parser.hpp"
#include "http_router.hpp"

#include <cstring>
#include <iostream>

#include <sys/socket.h>
#include <unistd.h>

HttpServer::HttpServer()
{
    server_fd = create_socket();

    sockaddr_in address = create_address();

    bind_socket(address);

    listen_socket();
}

HttpServer::~HttpServer()
{
    if (server_fd >= 0)
    {
        close(server_fd);
    }
}

void HttpServer::run()
{
    std::cout << "Server listening on port 8080...\n";

    while (true)
    {
        int client_fd = accept_client();

        if (client_fd < 0)
        {
            continue;
        }

        handle_client(client_fd);
    }
}

int HttpServer::create_socket()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0)
    {
        std::cerr << "Failed to create socket\n";
        exit(EXIT_FAILURE);
    }

    return fd;
}

sockaddr_in HttpServer::create_address()
{
    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;

    return address;
}

void HttpServer::bind_socket(const sockaddr_in& address)
{
    if (bind(server_fd,
             reinterpret_cast<const sockaddr*>(&address),
             sizeof(address)) < 0)
    {
        std::cerr << "Bind failed\n";
        close(server_fd);
        exit(EXIT_FAILURE);
    }
}

void HttpServer::listen_socket()
{
    if (listen(server_fd, 10) < 0)
    {
        std::cerr << "Listen failed\n";
        close(server_fd);
        exit(EXIT_FAILURE);
    }
}

int HttpServer::accept_client()
{
    sockaddr_in client_address{};
    socklen_t client_len = sizeof(client_address);

    int client_fd =
        accept(server_fd,
               reinterpret_cast<sockaddr*>(&client_address),
               &client_len);

    if (client_fd < 0)
    {
        std::cerr << "Accept failed\n";
    }

    return client_fd;
}

std::string HttpServer::receive_request(int client_fd)
{
    char buffer[4096];

    ssize_t bytes = recv(
        client_fd,
        buffer,
        sizeof(buffer),
        0);

    if (bytes <= 0)
        return "";

    return std::string(buffer, bytes);
}

void HttpServer::send_response(
    int client_fd,
    const HttpResponse& response)
{
    std::string data = response.to_string();

    send(
        client_fd,
        data.c_str(),
        data.size(),
        0);
}

void HttpServer::handle_client(int client_fd)
{
    std::string raw_request =
        receive_request(client_fd);

    if (raw_request.empty())
    {
        close(client_fd);
        return;
    }

  

    HttpRequest request =
        HttpParser::parse(raw_request);

    HttpResponse response =
        router.route(request);

    send_response(client_fd, response);

    close(client_fd);

    
}