#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "http/parser/parser.h"
#include "http/router.h"
#include "http/handlers/base_handler.h"
#include "http/handlers/json_handler.h"

// Shared user "database"
using UserStore = std::unordered_map<std::string, nlohmann::json>;

class UserPostHandler : public http::handlers::BaseHandler
{
public:
    UserPostHandler(UserStore &store) : users(store) {}
    std::string handle(const http::Request &req) const override
    {
        nlohmann::json resp;
        try
        {
            nlohmann::json posted = nlohmann::json::parse(req.body);
            if (!posted.contains("username"))
            {
                resp["success"] = false;
                resp["error"] = "Missing username";
                return resp.dump(2);
            }
            std::string username = posted["username"].get<std::string>();
            users[username] = posted;
            resp["success"] = true;
            resp["message"] = "User stored";
            resp["user"] = posted;
        }
        catch (const nlohmann::json::parse_error &)
        {
            resp["success"] = false;
            resp["error"] = "Invalid JSON in POST body";
        }
        return resp.dump(2);
    }

private:
    UserStore &users;
};

class UserGetHandler : public http::handlers::BaseHandler
{
public:
    UserGetHandler(const UserStore &store) : users(store) {}
    std::string handle(const http::Request &request) const override
    {
        nlohmann::json resp;
        auto username = util::get_param(request.query_params, "username");
        if (username && users.count(*username))
        {
            resp["success"] = true;
            resp["user"] = users.at(*username);
        }
        else
        {
            resp["success"] = false;
            resp["error"] = "User not found";
        }
        return resp.dump(2);
    }

private:
    const UserStore &users;
};

int main()
{
    http::Router router;

    // Simulated shared state
    UserStore users;

    // Register the GET and POST /user handlers using the shared state
    auto userGet = std::make_shared<UserGetHandler>(users);
    auto userPost = std::make_shared<UserPostHandler>(users);

    router.add_route(http::Method::GET, "/user", [userGet](const http::Request &req)
                     { return userGet->handle(req); });
    router.add_route(http::Method::POST, "/user", [userPost](const http::Request &req)
                     { return userPost->handle(req); });

    // First: POST /user with a new user
    {
        std::string raw_post =
            "POST /user HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: 55\r\n"
            "\r\n"
            "{\"username\":\"alice\",\"age\":31,\"email\":\"alice@demo.test\"}";
        http::Parser parser;
        if (parser.feed(raw_post.data(), raw_post.size()) && parser.message_complete)
        {
            std::cout << "\nPOST /user response:\n"
                      << router.route_request(parser.request) << std::endl;
        }
    }

    // Now: GET /user?username=alice should find the user just posted
    {
        std::string get_req =
            "GET /user?username=alice HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "\r\n";
        http::Parser parser;
        if (parser.feed(get_req.data(), get_req.size()) && parser.message_complete)
        {
            std::cout << "\nGET /user?username=alice response:\n"
                      << router.route_request(parser.request) << std::endl;
        }
    }

    // Test GET for non-existing user
    {
        std::string get_req =
            "GET /user?username=bob HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "\r\n";
        http::Parser parser;
        if (parser.feed(get_req.data(), get_req.size()) && parser.message_complete)
        {
            std::cout << "\nGET /user?username=bob response:\n"
                      << router.route_request(parser.request) << std::endl;
        }
    }

    return 0;
}
