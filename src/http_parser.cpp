#include "http_parser.hpp"

#include <algorithm>

HttpRequest HttpParser::parse(const std::string& raw_request)
{
    HttpRequest request;

    size_t pos = 0;

    //--------------------------------------------------
    // Request Line
    //--------------------------------------------------

    size_t line_end = raw_request.find("\r\n");

    if (line_end == std::string::npos)
        return request;

    size_t first_space = raw_request.find(' ');

    if (first_space == std::string::npos)
        return request;

    size_t second_space =
        raw_request.find(' ', first_space + 1);

    if (second_space == std::string::npos)
        return request;

    request.method =
        raw_request.substr(
            0,
            first_space);

    std::string url =
        raw_request.substr(
            first_space + 1,
            second_space - first_space - 1);

    request.version =
        raw_request.substr(
            second_space + 1,
            line_end - second_space - 1);

    parse_query(request, url);

    pos = line_end + 2;

    //--------------------------------------------------
    // Headers
    //--------------------------------------------------

    while (pos < raw_request.size())
    {
        size_t next =
            raw_request.find("\r\n", pos);

        if (next == std::string::npos)
            break;

        if (next == pos)
        {
            pos += 2;
            break;
        }

        size_t colon =
            raw_request.find(':', pos);

        if (colon != std::string::npos &&
            colon < next)
        {
            std::string key =
                raw_request.substr(
                    pos,
                    colon - pos);

            size_t value_start = colon + 1;

            while (value_start < next &&
                   raw_request[value_start] == ' ')
            {
                value_start++;
            }

            std::string value =
                raw_request.substr(
                    value_start,
                    next - value_start);

            request.headers[key] = value;
        }

        pos = next + 2;
    }

    //--------------------------------------------------
    // Body
    //--------------------------------------------------

    if (pos < raw_request.size())
    {
        request.body =
            raw_request.substr(pos);
    }

    //--------------------------------------------------
    // Content-Type
    //--------------------------------------------------

    auto it =
        request.headers.find("Content-Type");

    if (it != request.headers.end())
    {
        if (it->second.find(
                "application/x-www-form-urlencoded")
            != std::string::npos)
        {
            parse_form(request);
        }
        else if (it->second.find(
                     "application/json")
                 != std::string::npos)
        {
            parse_json(request);
        }
    }

    return request;
}
//--------------------------------------------------
// Parse Query String
//--------------------------------------------------

void HttpParser::parse_query(HttpRequest& request,
                             const std::string& url)
{
    size_t question = url.find('?');

    if (question == std::string::npos)
    {
        request.path = url;
        return;
    }

    request.path = url.substr(0, question);

    size_t pos = question + 1;

    while (pos < url.size())
    {
        size_t amp = url.find('&', pos);

        if (amp == std::string::npos)
            amp = url.size();

        size_t equals = url.find('=', pos);

        if (equals != std::string::npos &&
            equals < amp)
        {
            std::string key =
                url.substr(pos, equals - pos);

            std::string value =
                url.substr(equals + 1,
                           amp - equals - 1);

            request.query[key] = value;
        }
        else
        {
            std::string key =
                url.substr(pos, amp - pos);

            request.query[key] = "";
        }

        pos = amp + 1;
    }
}

//
//--------------------------------------------------
// Parse Form Data
//--------------------------------------------------

void HttpParser::parse_form(HttpRequest& request)
{
    const std::string& body = request.body;

    size_t pos = 0;

    while (pos < body.size())
    {
        size_t amp = body.find('&', pos);

        if (amp == std::string::npos)
            amp = body.size();

        size_t equals = body.find('=', pos);

        if (equals != std::string::npos &&
            equals < amp)
        {
            std::string key =
                body.substr(pos,
                            equals - pos);

            std::string value =
                body.substr(equals + 1,
                            amp - equals - 1);

            request.form[key] = value;
        }
        else
        {
            std::string key =
                body.substr(pos,
                            amp - pos);

            request.form[key] = "";
        }

        pos = amp + 1;
    }
}
//--------------------------------------------------
// Parse JSON
//--------------------------------------------------

void HttpParser::parse_json(HttpRequest& request)
{
    try
    {
        request.json =
            nlohmann::json::parse(request.body);
    }
    catch (...)
    {
        request.json =
            nlohmann::json::object();
    }
}