#pragma once

#include "http_request.hpp"

class HttpParser
{
public:
    static HttpRequest parse(const std::string& raw_request);

private:
    static void parse_query(HttpRequest& request,
                            const std::string& url);

    static void parse_form(HttpRequest& request);

    static void parse_json(HttpRequest& request);
};