#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "base_handler.h"
#include "http/request.h"

namespace http
{
    namespace handlers
    {

        /// Example JSON handler: returns a JSON-formatted response.
        class JsonHelloHandler : public BaseHandler
        {
        public:
            std::string handle(const http::Request &request) const override
            {
                nlohmann::json j;
                j["message"] = "Hello, world!";
                j["path"] = request.path;
                j["method"] = static_cast<int>(request.method); // For info
                return j.dump();                                // Serialize to string
            }
        };

    } // namespace handlers
} // namespace http
