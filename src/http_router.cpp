#include "http_router.hpp"
#include "handlers.hpp"

#include <sstream>


static bool match_route(
    const std::string& pattern,
    const std::string& path,
    HttpRequest& request)
{
    request.params.clear();

    size_t p = 0; // pattern index
    size_t s = 0; // path index

    while (true)
    {
        // Skip leading '/'
        while (p < pattern.size() && pattern[p] == '/')
            ++p;

        while (s < path.size() && path[s] == '/')
            ++s;

        // Both strings completely consumed
        if (p == pattern.size() && s == path.size())
            return true;

        // One finished before the other
        if (p == pattern.size() || s == path.size())
            return false;

        // Find end of current pattern segment
        size_t p_end = p;
        while (p_end < pattern.size() && pattern[p_end] != '/')
            ++p_end;

        // Find end of current path segment
        size_t s_end = s;
        while (s_end < path.size() && path[s_end] != '/')
            ++s_end;

        // Parameter segment (e.g. :id)
        if (pattern[p] == ':')
        {
            request.params.emplace(
                pattern.substr(p + 1, p_end - p - 1),
                path.substr(s, s_end - s));
        }
        else
        {
            // Static segment length must match
            size_t pattern_len = p_end - p;
            size_t path_len = s_end - s;

            if (pattern_len != path_len)
                return false;

            // Compare characters directly
            for (size_t i = 0; i < pattern_len; ++i)
            {
                if (pattern[p + i] != path[s + i])
                    return false;
            }
        }

        // Move to next segment
        p = p_end;
        s = s_end;
    }
}
Router::Router()
{
    get("/", Handlers::home);

    get("/hello", Handlers::hello);

    get("/health", Handlers::health);

    get("/users/:id", Handlers::user);

   
    post("/test", Handlers::test_post);
      get("/bench", Handlers::benchmark);
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