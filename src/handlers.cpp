#include "handlers.hpp"

HttpResponse Handlers::home(const HttpRequest&)
{
    HttpResponse response;

    response.body = "Home Page";

    return response;
}

HttpResponse Handlers::hello(const HttpRequest&)
{
    HttpResponse response;

    response.body = "Hello World";

    return response;
}

HttpResponse Handlers::health(const HttpRequest&)
{
    HttpResponse response;

    response.body = "OK";

    return response;
}

HttpResponse Handlers::not_found(const HttpRequest&)
{
    HttpResponse response;

    response.status_code = 404;
    response.status_text = "Not Found";
    response.body = "404 Not Found";

    return response;
}
HttpResponse Handlers::user(const HttpRequest& request)
{
    HttpResponse response;

    response.body = "User ID = " + request.params.at("id");

    return response;
}

HttpResponse Handlers::test_post(const HttpRequest& request)
{
    HttpResponse response;

    response.body =
        "Username: " + request.json["username"].get<std::string>() +
        "\nPassword: " + request.json["password"].get<std::string>();

    return response;
}