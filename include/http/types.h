#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>

namespace http
{

    // HTTP methods
    enum class Method
    {
        GET,
        POST,
        PUT,
        DELETE_,
        PATCH,
        HEAD,
        OPTIONS,
        TRACE,
        CONNECT,
        UNKNOWN
    };

    // HTTP version
    enum class Version
    {
        HTTP_1_0,
        HTTP_1_1,
        HTTP_2_0,
        UNKNOWN
    };

    // HTTP status codes (extend as needed)
    enum class StatusCode : uint16_t
    {
        OK = 200,
        BadRequest = 400,
        NotFound = 404,
        InternalServerError = 500,
        // Add others as needed
    };

    // Type alias for HTTP headers
    using Headers = std::unordered_map<std::string, std::string>;

    // Type alias for query parameters
    using QueryParams = std::unordered_map<std::string, std::string>;

    // Common constants
    constexpr char CRLF[] = "\r\n";
    constexpr char HEADER_SEPARATOR[] = ": ";

} // namespace http
