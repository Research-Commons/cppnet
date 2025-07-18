#include <gtest/gtest.h>
#include "../include/http/parser/parser.h"

// Description and expectation for each test case
struct HTTPTestCase
{
    std::string name; // Test case label for display
    std::string request;
    http::Method method;
    std::string path;
    std::string raw_url;
    http::Version version;
    std::string host;           // "" if not required to check
    std::string authorization;  // "" if not required to check
    std::string query_sort;     // "" if not required to check
    std::string query_filter;   // "" if not required to check
    std::string body_substring; // "" if not required to check
};

// Parameterized test fixture
class HTTPParserGTest : public ::testing::TestWithParam<HTTPTestCase>
{
};

TEST_P(HTTPParserGTest, ParsesVariousRequests)
{
    const HTTPTestCase &t = GetParam();

    http::Parser parser;
    ASSERT_TRUE(parser.feed(t.request.c_str(), t.request.size())) << "Parser feed failed for case: " << t.name;
    ASSERT_TRUE(parser.is_complete()) << "Parser incomplete for case: " << t.name;

    const http::Request &req = parser.get_request();

    EXPECT_EQ(req.method, t.method) << t.name;
    EXPECT_EQ(req.path, t.path) << t.name;
    EXPECT_EQ(req.raw_url, t.raw_url) << t.name;
    EXPECT_EQ(req.version, t.version) << t.name;

    if (!t.host.empty())
    {
        auto it = req.headers.find("host");
        ASSERT_TRUE(it != req.headers.end()) << "Missing header: host in " << t.name;
        EXPECT_EQ(it->second, t.host) << t.name;
    }
    if (!t.authorization.empty())
    {
        auto it = req.headers.find("authorization");
        ASSERT_TRUE(it != req.headers.end()) << "Missing header: authorization in " << t.name;
        EXPECT_EQ(it->second, t.authorization) << t.name;
    }
    if (!t.query_sort.empty())
    {
        auto it = req.query_params.find("sort");
        ASSERT_TRUE(it != req.query_params.end()) << "Missing query param: sort in " << t.name;
        EXPECT_EQ(it->second, t.query_sort) << t.name;
    }
    if (!t.query_filter.empty())
    {
        auto it = req.query_params.find("filter");
        ASSERT_TRUE(it != req.query_params.end()) << "Missing query param: filter in " << t.name;
        EXPECT_EQ(it->second, t.query_filter) << t.name;
    }
    if (!t.body_substring.empty())
    {
        EXPECT_NE(req.body.find(t.body_substring), std::string::npos)
            << "Missing expected body substring in " << t.name << ", got body: " << req.body;
    }
}

// Instantiate test cases
INSTANTIATE_TEST_SUITE_P(
    ParserVariants,
    HTTPParserGTest,
    ::testing::Values(
        HTTPTestCase{
            "PATCH, two queries, full headers, JSON body",
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
            "}",
            http::Method::PATCH, "/api/v2/resource/456", "/api/v2/resource/456?sort=desc&filter=active",
            http::Version::HTTP_1_1,
            "api.example.org", "Bearer ABCDEFGHIJKL123456", "desc", "active", "\"value\": \"Alice\""},
        HTTPTestCase{
            "GET, one query, minimal headers, empty body",
            "GET /foo/bar?token=xyz HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "\r\n",
            http::Method::GET, "/foo/bar", "/foo/bar?token=xyz",
            http::Version::HTTP_1_1,
            "localhost", "", "xyz", "", ""},
        HTTPTestCase{
            "POST, no query, minimal headers, with body",
            "POST /submit HTTP/1.1\r\n"
            "Host: server.example\r\n"
            "Content-Length: 11\r\n"
            "\r\n"
            "hello=world",
            http::Method::POST, "/submit", "/submit",
            http::Version::HTTP_1_1,
            "server.example", "", "", "", "hello=world"},
        HTTPTestCase{
            "PUT, no query, auth header, empty body",
            "PUT /data HTTP/1.1\r\n"
            "Host: 192.168.1.1\r\n"
            "Authorization: Bearer ABC\r\n"
            "\r\n",
            http::Method::PUT, "/data", "/data",
            http::Version::HTTP_1_1,
            "192.168.1.1", "Bearer ABC", "", "", ""},
        HTTPTestCase{
            "DELETE, one query, no headers, empty body",
            "DELETE /del?item=42 HTTP/1.1\r\n"
            "\r\n",
            http::Method::DELETE_, "/del", "/del?item=42",
            http::Version::HTTP_1_1,
            "", "", "42", "", ""}));
