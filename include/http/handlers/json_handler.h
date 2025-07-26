#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "http/request.h"
#include "http/handlers/base_handler.h"
#include "../../utils/query_params.h"

namespace http
{
    namespace handlers
    {

        // --- Original JSON GET Hello Handler ---
        class JsonHelloHandler : public BaseHandler
        {
        public:
            std::string handle(const http::Request &request) const override
            {
                nlohmann::json j;
                j["message"] = "Hello, world!";
                j["path"] = request.path;
                j["method"] = static_cast<int>(request.method);
                return j.dump();
            }
        };

        // --- GET: Echo query parameters as JSON ---
        class EchoGetHandler : public BaseHandler
        {
        public:
            std::string handle(const http::Request &request) const override
            {
                nlohmann::json response_json;
                int limit = util::get_with_default(request.query_params, "limit", 10);
                int page = util::get_with_default(request.query_params, "page", 1);
                auto sort = util::get_param(request.query_params, "sort");

                response_json["message"] = "GET processed";
                response_json["limit"] = limit;
                response_json["page"] = page;
                response_json["sort"] = sort.value_or("none");

                nlohmann::json all_params;
                for (const auto &[key, val] : request.query_params)
                    all_params[key] = val;
                response_json["all_query_params"] = all_params;

                return response_json.dump(2);
            }
        };

        // --- POST: Echo JSON body or error ---
        class EchoPostHandler : public BaseHandler
        {
        public:
            std::string handle(const http::Request &request) const override
            {
                nlohmann::json resp;
                try
                {
                    nlohmann::json posted = nlohmann::json::parse(request.body);
                    resp["message"] = "POST processed";
                    resp["received"] = posted;
                }
                catch (const nlohmann::json::parse_error &)
                {
                    resp["error"] = "Invalid JSON in POST body";
                }
                return resp.dump(2);
            }
        };

        // --- PUT: Return updated-like response ---
        class EchoPutHandler : public BaseHandler
        {
        public:
            std::string handle(const http::Request &request) const override
            {
                nlohmann::json resp;
                resp["message"] = "PUT processed";
                resp["body"] = request.body;
                resp["path"] = request.path;
                return resp.dump(2);
            }
        };

        // --- PATCH: Return partial update-like response ---
        class EchoPatchHandler : public BaseHandler
        {
        public:
            std::string handle(const http::Request &request) const override
            {
                nlohmann::json resp;
                resp["message"] = "PATCH processed";
                resp["body"] = request.body;
                return resp.dump(2);
            }
        };

        // --- DELETE: Indicate deletion of something ---
        class EchoDeleteHandler : public BaseHandler
        {
        public:
            std::string handle(const http::Request &request) const override
            {
                nlohmann::json resp;
                resp["message"] = "DELETE processed";
                resp["target"] = request.path;
                return resp.dump(2);
            }
        };

    } // namespace handlers
} // namespace http
