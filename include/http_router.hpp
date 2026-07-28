#pragma once

#include <functional>
#include <string>
#include <vector>

#include "http_request.hpp"
#include "http_response.hpp"

using Handler = std::function<HttpResponse(const HttpRequest&)>;

struct Route
{
    std::string method;
    std::string pattern;
    Handler handler;
};

class Router
{
public:
    Router();

    void get(
    const std::string& pattern,
    Handler handler);

void post(
    const std::string& pattern,
    Handler handler);

void put(
    const std::string& pattern,
    Handler handler);

void del(
    const std::string& pattern,
    Handler handler);

    HttpResponse route(HttpRequest& request);

private:
    std::vector<Route> routes;
};