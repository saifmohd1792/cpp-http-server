#pragma once

#include "http_request.hpp"
#include "http_response.hpp"

class Handlers
{
public:

    static HttpResponse home(const HttpRequest& request);

    static HttpResponse hello(const HttpRequest& request);

    static HttpResponse health(const HttpRequest& request);

    static HttpResponse not_found(const HttpRequest& request);

    static HttpResponse user(const HttpRequest& request);

    static HttpResponse test_post(
    const HttpRequest& request);

    static HttpResponse benchmark(const HttpRequest&);
};