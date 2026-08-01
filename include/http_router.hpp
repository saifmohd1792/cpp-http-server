#pragma once

#include <functional>
#include <string>
#include <vector>

#include "http_request.hpp"
#include "http_response.hpp"
#include"radix_tree.hpp"
#include "radix_node.hpp"
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
        const std::string& path,
        Handler handler);

    void post(
        const std::string& path,
        Handler handler);

    void put(
        const std::string& path,
        Handler handler);

    void del(
        const std::string& path,
        Handler handler);

    HttpResponse route(HttpRequest& request);

private:

    RadixTree get_tree_;
    RadixTree post_tree_;
    RadixTree put_tree_;
    RadixTree delete_tree_;
};