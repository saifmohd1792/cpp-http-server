#include "http_response.hpp"

#include <sstream>

std::string HttpResponse::to_string() const
{
    std::ostringstream response;

    // Status Line
    response << "HTTP/1.1 "
             << status_code
             << " "
             << status_text
             << "\r\n";

    // Required Headers
    response << "Content-Type: "
             << content_type
             << "\r\n";

    response << "Content-Length: "
             << body.size()
             << "\r\n";

    response << "Connection: close\r\n";

    // Custom Headers
    for (const auto& header : headers)
    {
        response << header.first
                 << ": "
                 << header.second
                 << "\r\n";
    }

    // Blank line separates headers from body
    response << "\r\n";

    // Response Body
    response << body;

    return response.str();
}