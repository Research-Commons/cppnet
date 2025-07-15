#pragma once
#include "llhttp.h"
#include <stddef.h>

namespace http
{

    // Forward declaration
    class Parser;

    namespace Callbacks
    {

        int on_message_begin(llhttp_t *parser);
        int on_url(llhttp_t *parser, const char *at, size_t length);
        int on_header_field(llhttp_t *parser, const char *at, size_t length);
        int on_header_value(llhttp_t *parser, const char *at, size_t length);
        int on_headers_complete(llhttp_t *parser);
        int on_body(llhttp_t *parser, const char *at, size_t length);
        int on_message_complete(llhttp_t *parser);

    } // namespace Callbacks
} // namespace http
