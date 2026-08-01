#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include "http_request.hpp"
#include "http_response.hpp"

using Handler = 
  std::function<HttpResponse(const HttpRequest&)>;

class RadixNode 
{
    public:
      std::string segment;
      bool parameter = false;
      bool terminal = false;

      Handler handler;
      std::unordered_map<
      std::string,std::unique_ptr<RadixNode>> children;
      std::unique_ptr<RadixNode> parameter_child;
};