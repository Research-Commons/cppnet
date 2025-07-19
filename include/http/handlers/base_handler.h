#pragma once

#include <string>
#include "http/request.h"

namespace http
{
    namespace handlers
    {

        /// Base class for any handler: JSON, Protobuf, etc.
        /// Users subclass this and implement the handle() method.
        /// (If you want, you can make this a pure interface with =0, but
        /// a default virtual destructor is sufficient for now.)
        class BaseHandler
        {
        public:
            virtual ~BaseHandler() = default;

            /// The main handler interface: process a request and return a response (as a string).
            /// The response will typically be a serialized payload (JSON/Protobuf).
            virtual std::string handle(const http::Request &request) const = 0;
        };

    } // namespace handlers
} // namespace http
