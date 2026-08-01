#include "radix_tree.hpp"

#include<sstream>

namespace 
{
    std::vector<std::string> split_path(const std::string&path)
    {
        std::vector<std::string> segments;
        std::stringstream ss(path);
        std::string segment;

        while(std::getline(ss,segment,'/'))
        {
            if(!segment.empty()) 
            {
                segments.push_back(segment);
            }
        }
        return segments;
    }
}
void RadixTree::insert (
    const std::string& path,Handler handler)
{
  auto segments = split_path(path);
  RadixNode* node = &root_;
  for(const auto& segment: segments)
  {
    if(!segment.empty() && segment.front() == ':')
    {
        if(!node->parameter_child)
        {
            node->parameter_child = std::make_unique<RadixNode>();
              node->parameter_child->segment =
                    segment.substr(1);

                node->parameter_child->parameter = true;
        }
        node = node->parameter_child.get();

    }

  
  else {
    auto& child = node->children[segment];
    if(!child)
    {
        child = std::make_unique<RadixNode>();
        child->segment = segment;
    }
    node = child.get();
  }
}
node->terminal = true;
node->handler = std::move(handler);
}
Handler RadixTree::find(HttpRequest& request)
{
    auto segments = split_path(request.path);

    RadixNode* node = &root_;

    request.params.clear();

    for (const auto& segment : segments)
    {
        auto it = node->children.find(segment);

        if (it != node->children.end())
        {
            node = it->second.get();
            continue;
        }

        if (node->parameter_child)
        {
            request.params[node->parameter_child->segment] = segment;
            node = node->parameter_child.get();
            continue;
        }

        return Handler{};
    }

    if (node->terminal)
    {
        return node->handler;
    }

    return Handler{};
}