#include <gtest/gtest.h>
#include "../include/http/parser/parser.h"

TEST(HttpParserTest, ParsesPatchRequestCorrectly)
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

    ASSERT_TRUE(parser.feed(http_request.c_str(), http_request.size()));
    ASSERT_TRUE(parser.is_complete());

    const http::Request &req = parser.get_request();

    // Debug: Uncomment these to see the keys received
    // for (const auto& h : req.headers)
    //     std::cout << "[HEADER] " << h.first << ": " << h.second << std::endl;
    // for (const auto& q : req.query_params)
    //     std::cout << "[QUERY ] " << q.first << ": " << q.second << std::endl;

    // Check that all keys exist before using .at()
    ASSERT_TRUE(req.headers.count("host")) << "Missing header: host";
    ASSERT_TRUE(req.headers.count("authorization")) << "Missing header: authorization";
    ASSERT_TRUE(req.query_params.count("sort")) << "Missing query param: sort";
    ASSERT_TRUE(req.query_params.count("filter")) << "Missing query param: filter";

    EXPECT_EQ(req.method, http::Method::PATCH);
    EXPECT_EQ(req.path, "/api/v2/resource/456");
    EXPECT_EQ(req.raw_url, "/api/v2/resource/456?sort=desc&filter=active");
    EXPECT_EQ(req.version, http::Version::HTTP_1_1);
    EXPECT_EQ(req.headers.at("host"), "api.example.org");
    EXPECT_EQ(req.headers.at("authorization"), "Bearer ABCDEFGHIJKL123456");
    EXPECT_EQ(req.query_params.at("sort"), "desc");
    EXPECT_EQ(req.query_params.at("filter"), "active");
    EXPECT_NE(req.body.find("\"value\": \"Alice\""), std::string::npos);
}
