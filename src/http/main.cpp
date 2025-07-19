#include <iostream>
#include <string>
#include "http/parser/parser.h"
#include "http/router.h"
#include "http/handlers/json_handler.h"

int main()
{
    // 1. Compose a raw HTTP request as it might arrive from a client (GET /hello)
    std::string raw_request =
        "GET /hello HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Connection: close\r\n"
        "\r\n";

    // 2. Parse the HTTP request using your parser
    http::Parser parser;
    bool parse_ok = parser.feed(raw_request.data(), raw_request.size());
    if (!parse_ok || !parser.message_complete)
    {
        std::cerr << "Failed to parse HTTP request!" << std::endl;
        return 1;
    }
    // Request object is now ready
    const http::Request &request = parser.request;

    // 3. Create router and register a JSON handler for GET /hello
    http::Router router;
    auto jsonHandler = std::make_shared<http::handlers::JsonHelloHandler>();
    router.add_route(http::Method::GET, "/hello",
                     [jsonHandler](const http::Request &req)
                     {
                         return jsonHandler->handle(req);
                     });

    // 4. Route the parsed request
    std::string response = router.route_request(request);
    std::cout << "Response to GET /hello:\n"
              << response << std::endl;

    // 5. Try an unregistered route to show 404
    // new parser for a different path
    std::string raw_unknown =
        "GET /unknown HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "\r\n";
    http::Parser parser2;
    bool parse2 = parser2.feed(raw_unknown.data(), raw_unknown.size());
    if (!parse2 || !parser2.message_complete)
    {
        std::cerr << "Failed to parse /unknown request!" << std::endl;
        return 1;
    }
    const http::Request &badRequest = parser2.request;
    std::string notFoundResponse = router.route_request(badRequest);
    std::cout << "Response to GET /unknown:\n"
              << notFoundResponse << std::endl;

    return 0;
}
