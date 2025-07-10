#pragma once

#include "../request.h"
#include <string>
#include <llhttp.h>

namespace http
{

    class Parser
    {
    public:
        Parser();
        ~Parser();

        // Reset parser state for a new HTTP message
        void reset();

        // Feed raw data to the parser; returns true if parsing was successful
        bool feed(const char *data, size_t length);

        // Returns true if the HTTP message is completely parsed
        bool is_complete() const { return message_complete; }

        // Access the parsed request object
        const Request &get_request() const { return request; }

        // Internal state for header parsing (used by callbacks)
        std::string last_header_field;
        std::string last_header_value;

        // The parsed HTTP request object
        Request request;

        // Message completion flag (set by callbacks)
        bool message_complete = false;

    private:
        llhttp_t parser_;
        llhttp_settings_t settings_;

        // Static llhttp callback functions
        static int on_message_begin(llhttp_t *parser);
        static int on_url(llhttp_t *parser, const char *at, size_t length);
        static int on_header_field(llhttp_t *parser, const char *at, size_t length);
        static int on_header_value(llhttp_t *parser, const char *at, size_t length);
        static int on_headers_complete(llhttp_t *parser);
        static int on_body(llhttp_t *parser, const char *at, size_t length);
        static int on_message_complete(llhttp_t *parser);

        // Helper to get the Parser* from llhttp_t
        static Parser *get_self(llhttp_t *parser);
    };

} // namespace http
