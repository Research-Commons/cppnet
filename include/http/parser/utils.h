#pragma once

#include <string>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <sstream>
#include "../include/http/types.h"

namespace http
{

    // Decodes a percent-encoded URL string
    std::string url_decode(const std::string &str);

    // Parses a query string (e.g., "a=1&b=2") into a QueryParams map
    QueryParams parse_query_string(const std::string &query);

    // Normalizes a header field to lowercase
    std::string normalize_header_field(const std::string &field);

    // Trims whitespace from both ends of a string
    std::string trim(const std::string &s);

} // namespace http
