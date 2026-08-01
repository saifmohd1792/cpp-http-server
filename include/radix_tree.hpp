#pragma once
#include "radix_node.hpp"
class RadixTree
{
public:

    void insert(
        const std::string& path,
        Handler handler);

    Handler find(
        HttpRequest& request);

private:

    RadixNode root_;
};