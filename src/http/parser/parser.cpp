#include "../include/http/parser/parser.h"
#include "../include/http/parser/callbacks.h"
#include "../include/http/parser/utils.h"
#include <cstring>
#include <iostream>
#include <stdexcept>

// random comment to mark successful commit

namespace http
{

    Parser::Parser()
    {
        llhttp_settings_init(&settings_);
        settings_.on_message_begin = &Parser::on_message_begin;
        settings_.on_url = &Parser::on_url;
        settings_.on_header_field = &Parser::on_header_field;
        settings_.on_header_value = &Parser::on_header_value;
        settings_.on_headers_complete = &Parser::on_headers_complete;
        settings_.on_body = &Parser::on_body;
        settings_.on_message_complete = &Parser::on_message_complete;

        llhttp_init(&parser_, HTTP_REQUEST, &settings_);
        parser_.data = this;

        request = Request();
        last_header_field.clear();
        last_header_value.clear();
        message_complete = false;
    }

    Parser::~Parser()
    {
        // No dynamic memory to clean up
    }

    void Parser::reset()
    {
        llhttp_reset(&parser_);
        parser_.data = this;

        request = Request();
        last_header_field.clear();
        last_header_value.clear();
        message_complete = false;
    }

    bool Parser::feed(const char *data, size_t length)
    {
        llhttp_errno_t err = llhttp_execute(&parser_, data, length);
        if (err != HPE_OK)
        {
            std::cerr << "llhttp error: " << llhttp_errno_name(err) << " - "
                      << llhttp_get_error_reason(&parser_) << std::endl;
            return false;
        }
        return true;
    }

    // ---- Static Callbacks ----

    Parser *Parser::get_self(llhttp_t *parser)
    {
        return static_cast<Parser *>(parser->data);
    }

    int Parser::on_message_begin(llhttp_t *parser)
    {
        // No-op, could reset state if needed
        return 0;
    }

    int Parser::on_url(llhttp_t *parser, const char *at, size_t length)
    {
        Parser *self = get_self(parser);
        return callbacks::on_url(*self, at, length);
    }

    int Parser::on_header_field(llhttp_t *parser, const char *at, size_t length)
    {
        Parser *self = get_self(parser);
        return callbacks::on_header_field(*self, at, length);
    }

    int Parser::on_header_value(llhttp_t *parser, const char *at, size_t length)
    {
        Parser *self = get_self(parser);
        return callbacks::on_header_value(*self, at, length);
    }

    int Parser::on_headers_complete(llhttp_t *parser)
    {
        Parser *self = get_self(parser);

        // Set HTTP version
        switch (parser->http_major)
        {
        case 1:
            self->request.version = (parser->http_minor == 1) ? Version::HTTP_1_1 : Version::HTTP_1_0;
            break;
        case 2:
            self->request.version = Version::HTTP_2_0;
            break;
        default:
            self->request.version = Version::UNKNOWN;
        }

        // Set HTTP method from llhttp using the robust string mapping
        self->request.method = http::callbacks::method_from_string(
            llhttp_method_name((llhttp_method_t)parser->method));

        return callbacks::on_headers_complete(*self);
    }

    int Parser::on_body(llhttp_t *parser, const char *at, size_t length)
    {
        Parser *self = get_self(parser);
        return callbacks::on_body(*self, at, length);
    }

    int Parser::on_message_complete(llhttp_t *parser)
    {
        Parser *self = get_self(parser);
        int ret = callbacks::on_message_complete(*self);
        self->message_complete = true;
        return ret;
    }

} // namespace http
