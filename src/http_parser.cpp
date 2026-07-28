#include "http_parser.hpp"
#include <nlohmann/json.hpp>

#include <sstream>

HttpRequest HttpParser::parse(const std::string& raw_request)
{
    HttpRequest request;
    

    std::istringstream stream(raw_request);

    std::string line;

    // -------- Request Line --------

    std::getline(stream, line);

    if (!line.empty() && line.back() == '\r')
        line.pop_back();

    std::istringstream request_line(line);

    std::string url;

    request_line
        >> request.method
        >> url
        >> request.version;

    // -------- Path + Query --------

    size_t question = url.find('?');

    if (question == std::string::npos)
    {
        request.path = url;
    }
    else
    {
        request.path = url.substr(0, question);

        std::string query = url.substr(question + 1);

        std::stringstream query_stream(query);

        std::string pair;

        while (std::getline(query_stream, pair, '&'))
        {
            size_t equals = pair.find('=');

            if (equals != std::string::npos)
            {
                request.query[
                    pair.substr(0, equals)
                ] = pair.substr(equals + 1);
            }
        }
    }

    // -------- Headers --------

    while (std::getline(stream, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line.empty())
            break;

        size_t colon = line.find(':');

        if (colon != std::string::npos)
        {
            std::string key = line.substr(0, colon);

            std::string value =
                line.substr(colon + 2);

            request.headers[key] = value;
        }
    }

    // -------- Body --------

    std::string body;

    while (std::getline(stream, line))
    {
        body += line;

        if (!stream.eof())
            body += "\n";
    }

    request.body = body;
auto it = request.headers.find("Content-Type");

if (it != request.headers.end())
{
    if (it->second.find("application/x-www-form-urlencoded")
        != std::string::npos)
    {
        parse_form(request);
    }
    else if (it->second.find("application/json")
             != std::string::npos)
    {
        parse_json(request);
    }
}

return request;
}
void HttpParser::parse_form(HttpRequest& request)
{
    std::stringstream stream(request.body);

    std::string pair;

    while (std::getline(stream, pair, '&'))
    {
        size_t equals = pair.find('=');

        if (equals == std::string::npos)
            continue;

        std::string key = pair.substr(0, equals);
        std::string value = pair.substr(equals + 1);

        request.form[key] = value;
    }
}

void HttpParser::parse_json(HttpRequest& request)
{
    try
    {
        request.json = nlohmann::json::parse(request.body);
    }
    catch (...)
    {
        request.json = nlohmann::json::object();
    }
}
