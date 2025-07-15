#pragma once

#include <string>
#include "../request.h"
#include "utils.h"
// random comment to mark succesful commit
namespace http
{

    // Forward declaration of the parser class (to be implemented later)
    class Parser;

    // Callback functions for HTTP parsing events
    namespace callbacks
    {
        Method method_from_string(const std::string &method_str);
        // Called when the HTTP method is parsed
        int on_method(Parser &parser, const std::string &method_str);

        // Called when URL data is parsed (may be called multiple times)
        int on_url(Parser &parser, const char *at, size_t length);

        // Called when a header field (name) is parsed (may be called multiple times)
        int on_header_field(Parser &parser, const char *at, size_t length);

        // Called when a header value is parsed (may be called multiple times)
        int on_header_value(Parser &parser, const char *at, size_t length);

        // Called when all headers are complete
        int on_headers_complete(Parser &parser);

        // Called when body data is parsed
        int on_body(Parser &parser, const char *at, size_t length);

        // Called when the entire message is complete
        int on_message_complete(Parser &parser);

    } // namespace callbacks

} // namespace http
