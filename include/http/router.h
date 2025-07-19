#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <memory>
#include "request.h"
#include "types.h"

namespace http
{

    // Base type for handler: accepts a Request, returns a response string (could be JSON, Protobuf, etc.)
    using HandlerFunc = std::function<std::string(const Request &)>;

    // Route key: combines method and path
    struct RouteKey
    {
        Method method;
        std::string path;

        bool operator==(const RouteKey &other) const
        {
            return method == other.method && path == other.path;
        }
    };

    // Custom hash function for RouteKey
    struct RouteKeyHash
    {
        std::size_t operator()(const RouteKey &k) const
        {
            return std::hash<int>()(static_cast<int>(k.method)) ^ std::hash<std::string>()(k.path);
        }
    };

    class Router
    {
    public:
        // Register a route: method + path -> handler
        void add_route(Method method, const std::string &path, HandlerFunc handler)
        {
            RouteKey key{method, path};
            routes_[key] = std::move(handler);
        }

        // Dispatch a request to the matching handler, else return "404"
        std::string route_request(const Request &req) const
        {
            RouteKey key{req.method, req.path};
            auto it = routes_.find(key);
            if (it != routes_.end())
            {
                return it->second(req);
            }
            return not_found_response();
        }

    private:
        std::unordered_map<RouteKey, HandlerFunc, RouteKeyHash> routes_;

        static std::string not_found_response()
        {
            // Plain 404; usually you'd want to template this to a proper HTTP response
            return "404 Not Found";
        }
    };

} // namespace http
