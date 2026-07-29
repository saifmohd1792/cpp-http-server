#include "http_server.hpp"
#include <nlohmann/json.hpp>
#include "epoll_server.hpp"

int main()
{
  EpollServer server(8080);

server.start();

    return 0;
}