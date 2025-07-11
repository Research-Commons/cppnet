#include <iostream>
#include <string>
#include "../include/http/parser/parser.h"

int main()
{
    http::Parser parser;

    // A simple HTTP GET request string
    std::string http_request = "GET /test?sort=desc HTTP/1.1\r\n"
                               "Host: example.com\r\n"
                               "Content-Type: application/json\r\n"
                               "Content-Length: 15\r\n"
                               "\r\n"
                               "{\"key\": \"value\"}";

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

    // Basic checks
    if (req.method != http::Method::GET)
    {
        std::cerr << "Method parsing failed." << std::endl;
        return 1;
    }
    if (req.path != "/test")
    {
        std::cerr << "Path parsing failed." << std::endl;
        return 1;
    }
    if (req.get_query_param("sort") != "desc")
    {
        std::cerr << "Query parameter parsing failed." << std::endl;
        return 1;
    }
    if (req.get_header("host") != "example.com")
    {
        std::cerr << "Header parsing failed." << std::endl;
        return 1;
    }
    if (req.get_header("content-type") != "application/json")
    {
        std::cerr << "Content-Type header parsing failed." << std::endl;
        return 1;
    }
    if (req.body != "{\"key\": \"value\"}")
    {
        std::cerr << "Body parsing failed." << std::endl;
        return 1;
    }

    std::cout << "Basic parser test passed successfully.\n"
              << std::endl;

    // Print all parsed parts
    std::cout << "---- Parsed HTTP Request ----" << std::endl;
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

    std::cout << "Path: " << req.path << std::endl;
    std::cout << "Raw URL: " << req.raw_url << std::endl;

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

    std::cout << "Headers:" << std::endl;
    for (const auto &h : req.headers)
    {
        std::cout << "  " << h.first << ": " << h.second << std::endl;
    }

    std::cout << "Query parameters:" << std::endl;
    for (const auto &q : req.query_params)
    {
        std::cout << "  " << q.first << ": " << q.second << std::endl;
    }

    std::cout << "Body: " << req.body << std::endl;

    return 0;
}
