#pragma once

#include <string>
#include "types.h"

namespace http
{

    class Request
    {
    public:
        // HTTP method (GET, POST, etc.)
        Method method = Method::UNKNOWN;

        // HTTP version (HTTP/1.1, etc.)
        Version version = Version::UNKNOWN;

        // Request path (e.g., "/api/resource")
        std::string path;

        // Raw URL (e.g., "/api/resource?sort=desc")
        std::string raw_url;

        // Query parameters (parsed from URL)
        QueryParams query_params;

        // HTTP headers
        Headers headers;

        // Request body (for POST, PUT, etc.)
        std::string body;

        // Remote address (optional, set by server)
        std::string remote_addr;

        // Utility: get a header value (case-insensitive), or empty if not found
        std::string get_header(const std::string &key) const;

        // Utility: get a query param value, or empty if not found
        std::string get_query_param(const std::string &key) const;
    };

} // namespace http
