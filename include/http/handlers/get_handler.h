#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "http/request.h"
#include "http/handlers/base_handler.h"
#include "util/query_params.h"

namespace http
{
    namespace handlers
    {

        /// Example GET handler: echoes query params as JSON.
        /// This can serve as a template for writing future resource handlers.
        class EchoGetHandler : public BaseHandler
        {
        public:
            std::string handle(const http::Request &request) const override
            {
                nlohmann::json response_json;

                // Use the utility to extract common query parameters
                int limit = util::get_with_default(request.query_params, "limit", 10);
                int page = util::get_with_default(request.query_params, "page", 1);
                auto sort = util::get_param(request.query_params, "sort");

                response_json["message"] = "GET processed";
                response_json["limit"] = limit;
                response_json["page"] = page;
                response_json["sort"] = sort.value_or("none");

                // Also reflect all other query parameters for demo purposes
                nlohmann::json all_params;
                for (const auto &[key, val] : request.query_params)
                    all_params[key] = val;
                response_json["all_query_params"] = all_params;

                return response_json.dump(2); // Pretty JSON for demo
            }
        };

    } // namespace handlers
} // namespace http
