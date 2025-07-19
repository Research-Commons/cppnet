#pragma once

#include <string>
#include "http/request.h"

namespace http
{
    namespace handlers
    {

        /// Base class for any handler: JSON, Protobuf, etc.
        /// Users subclass this and implement the handle() method.
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
