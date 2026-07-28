#pragma once

#include <string>
#include <unordered_map>

class HttpResponse
{
public:

    int status_code = 200;

    std::string status_text = "OK";

    std::string content_type = "text/plain";

    std::unordered_map<std::string, std::string> headers;

    std::string body;

    std::string to_string() const;
};