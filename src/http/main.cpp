#include <iostream>
#include <string>
#include <memory>
#include "http/parser/parser.h"
#include "http/router.h"
#include "http/handlers/json_handler.h"

// Add a dummy data handler inline for demonstration (simulate GET /user?username=alice)
class SimulatedUserGetHandler : public http::handlers::BaseHandler
{
public:
    std::string handle(const http::Request &request) const override
    {
        nlohmann::json resp;

        // Simulate some user "database"
        std::unordered_map<std::string, nlohmann::json> fake_users = {
            {"alice", {{"id", 1}, {"name", "Alice"}, {"email", "alice@example.com"}}},
            {"bob", {{"id", 2}, {"name", "Bob"}, {"email", "bob@example.com"}}}};

        auto username = util::get_param(request.query_params, "username");

        if (username && fake_users.count(*username))
        {
            resp["success"] = true;
            resp["user"] = fake_users.at(*username);
            return resp.dump(2);
        }
        else
        {
            resp["success"] = false;
            resp["error"] = "User not found";
            return resp.dump(2);
        }
    }
};

int main()
{
    http::Router router;

    // Register various example handlers for /echo with different methods
    auto helloHandler = std::make_shared<http::handlers::JsonHelloHandler>();
    auto getHandler = std::make_shared<http::handlers::EchoGetHandler>();
    auto postHandler = std::make_shared<http::handlers::EchoPostHandler>();
    auto putHandler = std::make_shared<http::handlers::EchoPutHandler>();
    auto patchHandler = std::make_shared<http::handlers::EchoPatchHandler>();
    auto delHandler = std::make_shared<http::handlers::EchoDeleteHandler>();
    auto userGet = std::make_shared<SimulatedUserGetHandler>();

    router.add_route(http::Method::GET, "/hello", [helloHandler](const http::Request &req)
                     { return helloHandler->handle(req); });

    router.add_route(http::Method::GET, "/echo", [getHandler](const http::Request &req)
                     { return getHandler->handle(req); });

    router.add_route(http::Method::POST, "/echo", [postHandler](const http::Request &req)
                     { return postHandler->handle(req); });

    router.add_route(http::Method::PUT, "/echo", [putHandler](const http::Request &req)
                     { return putHandler->handle(req); });

    router.add_route(http::Method::PATCH, "/echo", [patchHandler](const http::Request &req)
                     { return patchHandler->handle(req); });

    router.add_route(http::Method::DELETE_, "/echo", [delHandler](const http::Request &req)
                     { return delHandler->handle(req); });

    router.add_route(http::Method::GET, "/user", [userGet](const http::Request &req)
                     { return userGet->handle(req); });

    // Simulate a GET /echo request with query params
    {
        std::string raw_request =
            "GET /echo?limit=25&page=3&sort=asc HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "\r\n";
        http::Parser parser;
        if (parser.feed(raw_request.data(), raw_request.size()) && parser.message_complete)
        {
            std::cout << "\nGET /echo response:\n"
                      << router.route_request(parser.request) << std::endl;
        }
    }

    // Simulate a GET /user (simulated database)
    {
        std::string user_req =
            "GET /user?username=alice HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "\r\n";
        http::Parser parser;
        if (parser.feed(user_req.data(), user_req.size()) && parser.message_complete)
        {
            std::cout << "\nGET /user?username=alice response:\n"
                      << router.route_request(parser.request) << std::endl;
        }
    }

    // Simulate a POST /echo with a JSON body
    {
        // "Content-Length: 18" below is the size of the JSON body {"foo":42}
        std::string raw_request =
            "POST /echo HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: 11\r\n"
            "\r\n"
            "{\"foo\":42}";
        http::Parser parser;
        if (parser.feed(raw_request.data(), raw_request.size()) && parser.message_complete)
        {
            std::cout << "\nPOST /echo response:\n"
                      << router.route_request(parser.request) << std::endl;
        }
    }

    // Simulate a GET to unknown route (should show 404)
    {
        std::string unknown_req =
            "GET /unknown HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "\r\n";
        http::Parser parser;
        if (parser.feed(unknown_req.data(), unknown_req.size()) && parser.message_complete)
        {
            std::cout << "\nGET /unknown response:\n"
                      << router.route_request(parser.request) << std::endl;
        }
    }

    return 0;
}
