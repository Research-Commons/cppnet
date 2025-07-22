#include <iostream>
#include <string>
#include "http/parser/parser.h"
#include "http/router.h"
#include "http/handlers/get_handler.h"

int main()
{
    // Simulate a client GET request with query params (limit, page, sort)
    std::string raw_request =
        "GET /echo?limit=25&page=3&sort=asc HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Connection: close\r\n"
        "\r\n";

    // Parse the HTTP request string using the parser
    http::Parser parser;
    bool parse_ok = parser.feed(raw_request.data(), raw_request.size());
    if (!parse_ok || !parser.message_complete)
    {
        std::cerr << "Failed to parse HTTP request." << std::endl;
        return 1;
    }
    const http::Request &request = parser.request;

    // Register GET /echo with the EchoGetHandler
    http::Router router;
    auto echoHandler = std::make_shared<http::handlers::EchoGetHandler>();
    router.add_route(http::Method::GET, "/echo",
                     [echoHandler](const http::Request &req)
                     {
                         return echoHandler->handle(req);
                     });

    // Dispatch the parsed request
    std::string response = router.route_request(request);
    std::cout << "Response to GET /echo:\n"
              << response << std::endl;

    // Now try a request to a non-registered route (/unknown) to verify 404 case
    std::string raw_unknown =
        "GET /unknown HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "\r\n";
    http::Parser parser2;
    bool parse2 = parser2.feed(raw_unknown.data(), raw_unknown.size());
    if (!parse2 || !parser2.message_complete)
    {
        std::cerr << "Failed to parse /unknown request." << std::endl;
        return 1;
    }
    const http::Request &badRequest = parser2.request;
    std::string notFoundResponse = router.route_request(badRequest);
    std::cout << "Response to GET /unknown:\n"
              << notFoundResponse << std::endl;

    return 0;
}
