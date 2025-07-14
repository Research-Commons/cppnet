#include "../include/http/parser/callbacks.h"
#include "../include/http/parser/parser.h"
#include <cstring>
#include <iostream>

namespace http
{

    namespace callbacks
    {

        // Helper function to convert method string to enum
        static Method method_from_string(const std::string &method_str)
        {
            if (method_str == "GET")
                return Method::GET;
            if (method_str == "POST")
                return Method::POST;
            if (method_str == "PUT")
                return Method::PUT;
            if (method_str == "DELETE")
                return Method::DELETE_;
            if (method_str == "PATCH")
                return Method::PATCH;
            if (method_str == "HEAD")
                return Method::HEAD;
            if (method_str == "OPTIONS")
                return Method::OPTIONS;
            if (method_str == "TRACE")
                return Method::TRACE;
            if (method_str == "CONNECT")
                return Method::CONNECT;
            return Method::UNKNOWN;
        }

        int on_method(Parser &parser, const std::string &method_str)
        {
            parser.request.method = method_from_string(method_str);
            return 0;
        }

        int on_url(Parser &parser, const char *at, size_t length)
        {
            parser.request.raw_url.append(at, length);

            // Find query string start
            size_t qs_pos = parser.request.raw_url.find('?');
            if (qs_pos != std::string::npos)
            {
                parser.request.path = parser.request.raw_url.substr(0, qs_pos);
                std::string query_str = parser.request.raw_url.substr(qs_pos + 1);
                parser.request.query_params = parse_query_string(query_str);
            }
            else
            {
                parser.request.path = parser.request.raw_url;
                parser.request.query_params.clear();
            }

            return 0;
        }

        int on_header_field(Parser &parser, const char *at, size_t length)
        {
            std::string field(at, length);
            if (!parser.last_header_value.empty())
            {
                // Store the previous header field-value pair
                parser.request.headers[normalize_header_field(parser.last_header_field)] = trim(parser.last_header_value);
                parser.last_header_field.clear();
                parser.last_header_value.clear();
            }
            parser.last_header_field.append(field);
            return 0;
        }

        int on_header_value(Parser &parser, const char *at, size_t length)
        {
            std::string value(at, length);
            parser.last_header_value.append(value);
            return 0;
        }

        int on_headers_complete(Parser &parser)
        {
            if (!parser.last_header_field.empty())
            {
                parser.request.headers[normalize_header_field(parser.last_header_field)] = trim(parser.last_header_value);
                parser.last_header_field.clear();
                parser.last_header_value.clear();
            }

            // Corrected: cast parser.parser_.method to const char* for both ternary and string construction
            const char *method_ptr = reinterpret_cast<const char *>(parser.parser_.method);
            std::cout << "parser.parser_.method: "
                      << (method_ptr ? method_ptr : "NULL") << std::endl;

            if (method_ptr)
            {
                parser.request.method = method_from_string(std::string(method_ptr));
            }
            else
            {
                parser.request.method = Method::UNKNOWN;
                std::cerr << "[ERROR] parser_.method is null in on_headers_complete\n";
            }
            return 0;
        }

        int on_body(Parser &parser, const char *at, size_t length)
        {
            parser.request.body.append(at, length);
            return 0;
        }

        int on_message_complete(Parser &parser)
        {
            parser.message_complete = true;
            return 0;
        }

    } // namespace callbacks

} // namespace http
