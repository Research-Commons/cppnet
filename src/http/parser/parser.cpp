#include "llhttp.h"
#include <string>
#include <iostream>
#include <vector>

namespace http
{

    class Parser
    {
    public:
        Parser();
        void reset();
        bool feed(const char *data, std::size_t length);

        // Your desired data fields
        std::string method_str;
        std::string url;
        std::vector<std::pair<std::string, std::string>> headers;
        std::string body;

    private:
        llhttp_t parser_;
        llhttp_settings_t settings_;

        // Static wrapper callbacks
        static int on_url(llhttp_t *parser, const char *at, size_t length);
        static int on_header_field(llhttp_t *parser, const char *at, size_t length);
        static int on_header_value(llhttp_t *parser, const char *at, size_t length);
        static int on_body(llhttp_t *parser, const char *at, size_t length);
        static int on_message_begin(llhttp_t *parser);
        static int on_headers_complete(llhttp_t *parser);
        static int on_message_complete(llhttp_t *parser);

        // Track state for header parsing
        std::string last_header_field_;
        bool in_value_ = false;
    };

    Parser::Parser()
    {
        llhttp_settings_init(&settings_);
        settings_.on_url = &Parser::on_url;
        settings_.on_header_field = &Parser::on_header_field;
        settings_.on_header_value = &Parser::on_header_value;
        settings_.on_body = &Parser::on_body;
        settings_.on_message_begin = &Parser::on_message_begin;
        settings_.on_headers_complete = &Parser::on_headers_complete;
        settings_.on_message_complete = &Parser::on_message_complete;

        llhttp_init(&parser_, HTTP_REQUEST, &settings_);
        parser_.data = this;

        // Clear all previous state
        method_str.clear();
        url.clear();
        headers.clear();
        body.clear();
        last_header_field_.clear();
        in_value_ = false;
    }

    void Parser::reset()
    {
        llhttp_reset(&parser_);
        parser_.data = this;

        method_str.clear();
        url.clear();
        headers.clear();
        body.clear();
        last_header_field_.clear();
        in_value_ = false;
    }

    bool Parser::feed(const char *data, std::size_t length)
    {
        llhttp_errno_t err = llhttp_execute(&parser_, data, length);
        if (err != HPE_OK)
        {
            std::cerr << "llhttp error: " << llhttp_errno_name(err) << " - ";
            if (parser_.reason)
            {
                std::cerr << parser_.reason;
            }
            std::cerr << std::endl;
            return false;
        }
        return true;
    }

    // Empty definitions for static callback functions to satisfy the linker.
    // You will want to fill these in later.

    int Parser::on_url(llhttp_t *parser, const char *at, size_t length) { return 0; }
    int Parser::on_header_field(llhttp_t *parser, const char *at, size_t length) { return 0; }
    int Parser::on_header_value(llhttp_t *parser, const char *at, size_t length) { return 0; }
    int Parser::on_body(llhttp_t *parser, const char *at, size_t length) { return 0; }
    int Parser::on_message_begin(llhttp_t *parser) { return 0; }
    int Parser::on_headers_complete(llhttp_t *parser) { return 0; }
    int Parser::on_message_complete(llhttp_t *parser) { return 0; }

} // namespace http
