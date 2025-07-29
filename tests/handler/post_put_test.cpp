// test_post_get_put_get.cpp
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "http/parser/parser.h"
#include "http/router.h"
#include "http/handlers/base_handler.h"
#include "http/handlers/json_handler.h"
#include "include/utils/query_params.h"

using UserStore = std::unordered_map<std::string, nlohmann::json>;

// PUT handler: replaces the full user data for the username
class UserPutHandler : public http::handlers::BaseHandler
{
public:
    UserPutHandler(UserStore &store) : users(store) {}
    std::string handle(const http::Request &req) const override
    {
        nlohmann::json resp;
        auto username = util::get_param(req.query_params, "username");
        if (!username)
        {
            resp["success"] = false;
            resp["error"] = "Missing username for PUT";
            return resp.dump(2);
        }
        try
        {
            nlohmann::json replacement = nlohmann::json::parse(req.body);
            users[*username] = replacement;
            resp["success"] = true;
            resp["message"] = "User put/updated";
            resp["user"] = replacement;
        }
        catch (...)
        {
            resp["success"] = false;
            resp["error"] = "Invalid JSON in PUT body";
        }
        return resp.dump(2);
    }

private:
    UserStore &users;
};

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
        catch (...)
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
    UserStore users;

    auto userGet = std::make_shared<UserGetHandler>(users);
    auto userPost = std::make_shared<UserPostHandler>(users);
    auto userPut = std::make_shared<UserPutHandler>(users);

    router.add_route(http::Method::POST, "/user", [userPost](const http::Request &req)
                     { return userPost->handle(req); });
    router.add_route(http::Method::GET, "/user", [userGet](const http::Request &req)
                     { return userGet->handle(req); });
    router.add_route(http::Method::PUT, "/user", [userPut](const http::Request &req)
                     { return userPut->handle(req); });

    // POST alice
    std::string raw_post =
        "POST /user HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 55\r\n"
        "\r\n"
        "{\"username\":\"alice\",\"age\":31,\"email\":\"alice@demo.test\"}";
    http::Parser parser1;
    parser1.feed(raw_post.data(), raw_post.size());
    std::cout << "\nPOST /user response:\n"
              << router.route_request(parser1.request) << std::endl;

    // GET alice
    std::string get_alice =
        "GET /user?username=alice HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "\r\n";
    http::Parser parser2;
    parser2.feed(get_alice.data(), get_alice.size());
    std::cout << "\nGET /user?username=alice response:\n"
              << router.route_request(parser2.request) << std::endl;

    // PUT alice (replace with a new record)
    std::string put_alice =
        "PUT /user?username=alice HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 69\r\n"
        "\r\n"
        "{\"username\":\"alice\",\"age\":40,\"email\":\"alice@foo.com\",\"vip\":true}";
    http::Parser parser3;
    parser3.feed(put_alice.data(), put_alice.size());
    std::cout << "\nPUT /user?username=alice response:\n"
              << router.route_request(parser3.request) << std::endl;

    // GET again — should reflect replaced data
    http::Parser parser4;
    parser4.feed(get_alice.data(), get_alice.size());
    std::cout << "\nGET /user?username=alice response:\n"
              << router.route_request(parser4.request) << std::endl;

    return 0;
}
