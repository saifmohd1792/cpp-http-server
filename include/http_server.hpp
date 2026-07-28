#pragma once

#include <arpa/inet.h>
#include <string>

#include "http_response.hpp"
#include "http_router.hpp"
// Forward declarations
class HttpResponse;

class HttpServer
{
public:
    HttpServer();
    ~HttpServer();

    void run();

private:
    int server_fd;
    Router router;
    int create_socket();

    sockaddr_in create_address();

    void bind_socket(const sockaddr_in& address);

    void listen_socket();

    int accept_client();

    std::string receive_request(int client_fd);

    void send_response(
        int client_fd,
        const HttpResponse& response);

    void handle_client(int client_fd);
};