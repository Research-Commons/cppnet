// test_post_get_patch_get.cpp
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "http/parser/parser.h"
#include "http/router.h"
#include "http/handlers/base_handler.h"
#include "http/handlers/json_handler.h"
#include "../../utils/query_params.h"

using UserStore = std::unordered_map<std::string, nlohmann::json>;

// PATCH handler: partial update, only updates fields present in body
class UserPatchHandler : public http::handlers::BaseHandler
{
public:
    UserPatchHandler(UserStore &store) : users(store) {}
    std::string handle(const http::Request &req) const override
    {
        nlohmann::json resp;
        auto username = util::get_param(req.query_params, "username");
        if (!(username && users.count(*username)))
        {
            resp["success"] = false;
            resp["error"] = "User not found";
            return resp.dump(2);
        }
        try
        {
            nlohmann::json patch = nlohmann::json::parse(req.body);
            // Only update fields present in patch
            for (auto it = patch.begin(); it != patch.end(); ++it)
                users[*username][it.key()] = it.value();
            resp["success"] = true;
            resp["message"] = "User patched";
            resp["user"] = users[*username];
        }
        catch (...)
        {
            resp["success"] = false;
            resp["error"] = "Invalid JSON in PATCH body";
        }
        return resp.dump(2);
    }

private:
    UserStore &users;
};

// ... UserPostHandler & UserGetHandler same as above (reuse or copy)

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
    auto userPatch = std::make_shared<UserPatchHandler>(users);

    router.add_route(http::Method::POST, "/user", [userPost](const http::Request &req)
                     { return userPost->handle(req); });
    router.add_route(http::Method::GET, "/user", [userGet](const http::Request &req)
                     { return userGet->handle(req); });
    router.add_route(http::Method::PATCH, "/user", [userPatch](const http::Request &req)
                     { return userPatch->handle(req); });

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

    // PATCH alice (update her age only)
    std::string patch_alice =
        "PATCH /user?username=alice HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "{\"age\":35}";
    http::Parser parser3;
    parser3.feed(patch_alice.data(), patch_alice.size());
    std::cout << "\nPATCH /user?username=alice response:\n"
              << router.route_request(parser3.request) << std::endl;

    // GET again — should reflect patched age
    http::Parser parser4;
    parser4.feed(get_alice.data(), get_alice.size());
    std::cout << "\nGET /user?username=alice response:\n"
              << router.route_request(parser4.request) << std::endl;

    return 0;
}
