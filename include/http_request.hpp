#pragma once

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
class HttpRequest
{
public:

    std::string method;

    std::string path;

    std::string version;

    std::unordered_map<
        std::string,
        std::string> headers;

    std::unordered_map<
        std::string,
        std::string> query;

    std::string body;

    std::unordered_map<std::string, std::string> params;
    std::unordered_map<std::string,std::string> form;
    nlohmann::json json;

};