#include "http_server.hpp"
#include <nlohmann/json.hpp>


int main()
{
    HttpServer server;

    server.run();

    return 0;
}