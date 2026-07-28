#include "http_router.hpp"
#include "handlers.hpp"

#include <sstream>

static std::vector<std::string> split(const std::string& path)
{
    std::vector<std::string> parts;

    std::stringstream ss(path);

    std::string part;

    while (std::getline(ss, part, '/'))
    {
        if (!part.empty())
            parts.push_back(part);
    }

    return parts;
}

static bool match_route(
    const std::string& pattern,
    const std::string& path,
    HttpRequest& request)
{
    auto pattern_parts = split(pattern);
    auto path_parts = split(path);

    if (pattern_parts.size() != path_parts.size())
        return false;

    request.params.clear();

    for (size_t i = 0; i < pattern_parts.size(); i++)
    {
        const auto& pattern_part = pattern_parts[i];
        const auto& path_part = path_parts[i];

        if (!pattern_part.empty() && pattern_part[0] == ':')
        {
            request.params[pattern_part.substr(1)] = path_part;
        }
        else if (pattern_part != path_part)
        {
            return false;
        }
    }

    return true;
}
Router::Router()
{
    get("/", Handlers::home);

    get("/hello", Handlers::hello);

    get("/health", Handlers::health);

    get("/users/:id", Handlers::user);

   
    post("/test", Handlers::test_post);
}

void Router::get(
    const std::string& pattern,
    Handler handler)
{
    routes.push_back(
        {"GET", pattern, handler});
}

void Router::post(
    const std::string& pattern,
    Handler handler)
{
    routes.push_back(
        {"POST", pattern, handler});
}

void Router::put(
    const std::string& pattern,
    Handler handler)
{
    routes.push_back(
        {"PUT", pattern, handler});
}

void Router::del(
    const std::string& pattern,
    Handler handler)
{
    routes.push_back(
        {"DELETE", pattern, handler});
}

HttpResponse Router::route(HttpRequest& request)
{
    for (const auto& route : routes)
    {
        if (route.method != request.method)
{
    continue;
}

if (match_route(
        route.pattern,
        request.path,
        request))
{
    return route.handler(request);
}
    }

    return Handlers::not_found(request);
}