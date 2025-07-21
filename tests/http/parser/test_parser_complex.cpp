#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "../include/http/parser/parser.h"

int main()
{
    http::Parser parser;

    std::string http_request =
        "PATCH /api/v2/resource/456?sort=desc&filter=active HTTP/1.1\r\n"
        "Host: api.example.org\r\n"
        "Accept: application/json\r\n"
        "Content-Type: application/json\r\n"
        "X-Request-ID: 123e4567-e89b-12d3-a456-426614174000\r\n"
        "Authorization: Bearer ABCDEFGHIJKL123456\r\n"
        "Content-Length: 114\r\n"
        "\r\n"
        "{"
        "\"user\": {\"id\": 42, \"roles\": [\"admin\",\"user\"]}, "
        "\"updates\": [{\"op\": \"replace\", \"path\": \"/name\", \"value\": \"Alice\"}]"
        "}";

    bool success = parser.feed(http_request.c_str(), http_request.size());
    if (!success)
    {
        std::cerr << "Parsing failed." << std::endl;
        return 1;
    }

    if (!parser.is_complete())
    {
        std::cerr << "Parsing incomplete." << std::endl;
        return 1;
    }

    const http::Request &req = parser.get_request();

    // Print all parsed parts
    std::cout << "---- Parsed HTTP Request ----" << std::endl;

    // Print Method
    std::cout << "Method: ";
    switch (req.method)
    {
    case http::Method::GET:
        std::cout << "GET";
        break;
    case http::Method::POST:
        std::cout << "POST";
        break;
    case http::Method::PUT:
        std::cout << "PUT";
        break;
    case http::Method::DELETE_:
        std::cout << "DELETE";
        break;
    case http::Method::PATCH:
        std::cout << "PATCH";
        break;
    case http::Method::HEAD:
        std::cout << "HEAD";
        break;
    case http::Method::OPTIONS:
        std::cout << "OPTIONS";
        break;
    case http::Method::TRACE:
        std::cout << "TRACE";
        break;
    case http::Method::CONNECT:
        std::cout << "CONNECT";
        break;
    default:
        std::cout << "UNKNOWN";
        break;
    }
    std::cout << std::endl;

    // Print Path
    std::cout << "Path: " << req.path << std::endl;
    std::cout << "Raw URL: " << req.raw_url << std::endl;

    // HTTP Version
    std::cout << "HTTP Version: ";
    switch (req.version)
    {
    case http::Version::HTTP_1_0:
        std::cout << "HTTP/1.0";
        break;
    case http::Version::HTTP_1_1:
        std::cout << "HTTP/1.1";
        break;
    case http::Version::HTTP_2_0:
        std::cout << "HTTP/2.0";
        break;
    default:
        std::cout << "UNKNOWN";
        break;
    }
    std::cout << std::endl;

    // Print Headers
    std::cout << "Headers:" << std::endl;
    for (const auto &h : req.headers)
    {
        std::cout << "  " << h.first << ": " << h.second << std::endl;
    }

    // Print Query Parameters
    std::cout << "Query parameters:" << std::endl;
    for (const auto &q : req.query_params)
    {
        std::cout << "  " << q.first << ": " << q.second << std::endl;
    }

    // Print Body
    std::cout << "Body: " << req.body << std::endl;

    // --- PARSE AND PRINT THE JSON BODY USING NLOHMANN::JSON ---
    try
    {
        nlohmann::json body = nlohmann::json::parse(req.body);
        std::cout << "\n--- Deserialized JSON Body ---" << std::endl;
        std::cout << body.dump(2) << std::endl;

        // Example: access fields individually
        if (body.contains("user"))
        {
            auto user = body["user"];
            std::cout << "User id: " << user["id"] << std::endl;
            std::cout << "User roles: ";
            for (const auto &r : user["roles"])
                std::cout << r << " ";
            std::cout << std::endl;
        }
        if (body.contains("updates"))
        {
            std::cout << "Updates:" << std::endl;
            for (const auto &upd : body["updates"])
            {
                std::cout << "  op: " << upd["op"]
                          << ", path: " << upd["path"]
                          << ", value: " << upd["value"] << std::endl;
            }
        }
    }
    catch (const nlohmann::json::parse_error &e)
    {
        std::cerr << "Failed to parse JSON body: " << e.what() << std::endl;
    }
    // ----------------------------------------
    return 0;
}
