#include "parser.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace crequests;

TEST(Parser, StatusAcquire) {
    parser_t parser(parser_t::parser_type_t::RESPONSE);

    const char* data =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Accept: */*\r\n"
        "Accept-Encoding: gzip, deflate\r\n\r\n";

    unsigned short http_major {};
    unsigned short http_minor {};
    unsigned int status_code {};
    std::string status_message {};
    
    auto status_fn = [&](const char* at,
                         size_t length,
                         unsigned short major,
                         unsigned short minor,
                         unsigned int code) {
        status_message.reserve(length);
        status_message.assign(at, length);
        http_major = major;
        http_minor = minor;
        status_code = code;
    };

    parser.bind_cb(status_fn);
    auto rv = strlen(data) == parser.execute(data, strlen(data));

    EXPECT_TRUE(rv);
    EXPECT_EQ(http_major, 1);
    EXPECT_EQ(http_minor, 1);
    EXPECT_EQ(status_code, 200);
    EXPECT_EQ(status_message, "OK");
}

TEST(Parser, HeaderFieldAcquire) {
    parser_t parser(parser_t::parser_type_t::RESPONSE);

    const char* data =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Accept: */*\r\n"
        "Accept-Encoding: gzip, deflate\r\n\r\n";

    std::string header_field;
    auto header_field_fn = [&](const char* at,
                               size_t length) {
        header_field.reserve(length);
        header_field.assign(at, length);
    };

    parser.bind_cb(parser_t::HEADER_FIELD, header_field_fn);
    auto rv = strlen(data) == parser.execute(data, strlen(data));

    EXPECT_TRUE(rv);
    EXPECT_EQ(header_field, "Accept-Encoding");
}

TEST(Parser, HeaderValueAcquire) {
    parser_t parser(parser_t::parser_type_t::RESPONSE);

    const char* data =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Accept: */*\r\n"
        "Accept-Encoding: gzip, deflate\r\n\r\n";

    std::string header_value;
    auto header_value_fn = [&](const char* at,
                               size_t length) {
        header_value.reserve(length);
        header_value.assign(at, length);
    };

    parser.bind_cb(parser_t::HEADER_VALUE, header_value_fn);
    auto rv = strlen(data) == parser.execute(data, strlen(data));

    EXPECT_TRUE(rv);
    EXPECT_EQ(header_value, "gzip, deflate");
}

TEST(Parser, FeedHeadersConsequencelly) {
    parser_t parser(parser_t::parser_type_t::RESPONSE);

    const std::vector<std::string> v = {
        "HTTP/1.1 200 OK\r\n",
        "Connection: close\r\n",
        "Accept: */*\r\n",
        "Accept-Encoding: ",
        "gzip, deflate",
        "\r\n\r\n"
    };

    for (auto& data : v) {
        auto rv = data.size() == parser.execute(data.c_str(), data.size());
        EXPECT_TRUE(rv);
    }
}

TEST(Parser, BadStatusHttp) {
    parser_t parser(parser_t::parser_type_t::RESPONSE);

    const char* data = "HTT/1.1 200 OK\r\n";
    auto rv = strlen(data) == parser.execute(data, strlen(data));
    
    EXPECT_FALSE(rv);
}

TEST(Parser, BadStatusMajor) {
    parser_t parser(parser_t::parser_type_t::RESPONSE);

    const char* data = "HTTP/.1 200 OK\r\n";
    auto rv = strlen(data) == parser.execute(data, strlen(data));
    
    EXPECT_FALSE(rv);
}

TEST(Parser, BadStatusMinor) {
    parser_t parser(parser_t::parser_type_t::RESPONSE);

    const char* data = "HTTP/1 200 OK\r\n";
    auto rv = strlen(data) == parser.execute(data, strlen(data));
    
    EXPECT_FALSE(rv);
}

TEST(Parser, BadStatusCode) {
    parser_t parser(parser_t::parser_type_t::RESPONSE);

    const char* data = "HTTP/1.1 OK\r\n";
    auto rv = strlen(data) == parser.execute(data, strlen(data));
    
    EXPECT_FALSE(rv);
}

TEST(Parser, BadHeaders) {
    parser_t parser(parser_t::parser_type_t::RESPONSE);

    const char* data =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r"
        "Accept: */*\r\n"
        "Accept-Encoding: gzip, deflate\r\n\r\n";

    auto rv = strlen(data) == parser.execute(data, strlen(data));

    EXPECT_FALSE(rv);
}

TEST(Parser, BadChunkHeaderSymbol) {
    parser_t parser(parser_t::parser_type_t::RESPONSE);

    const char* data =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Transfer-Encoding: chunked\r\n"
        "Accept: */*\r\n"
        "Accept-Encoding: gzip, deflate\r\n\r\n"
        "123abc^";

    auto rv = strlen(data) == parser.execute(data, strlen(data));

    EXPECT_FALSE(rv);
}

TEST(Parser, BadChunkDataValue) {
    parser_t parser(parser_t::parser_type_t::RESPONSE);

    const char* data =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Transfer-Encoding: chunked\r\n"
        "Accept: */*\r\n"
        "Accept-Encoding: gzip, deflate\r\n\r\n"
        "2\r\n"
        "qqq\r\n";

    auto rv = strlen(data) == parser.execute(data, strlen(data));

    EXPECT_FALSE(rv);
}

TEST(Parser, BadChunkHeaderValue) {
    parser_t parser(parser_t::parser_type_t::RESPONSE);

    const char* data =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Transfer-Encoding: chunked\r\n"
        "Accept: */*\r\n"
        "Accept-Encoding: gzip, deflate\r\n\r\n"
        "2\n"
        "qqq\r\n";

    auto rv = strlen(data) == parser.execute(data, strlen(data));

    EXPECT_FALSE(rv);
}

TEST(Parser, GoodChunkDataValue) {
    parser_t parser(parser_t::parser_type_t::RESPONSE);

    const char* data =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Transfer-Encoding: chunked\r\n"
        "Accept: */*\r\n"
        "Accept-Encoding: gzip, deflate\r\n\r\n"
        "2\r\n"
        "qq\r\n";

    auto rv = strlen(data) == parser.execute(data, strlen(data));

    EXPECT_TRUE(rv);
}

TEST(Parser, ChunkHeaderAcquire) {
    parser_t parser(parser_t::parser_type_t::RESPONSE);

    const char* data =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Transfer-Encoding: chunked\r\n"
        "Accept: */*\r\n"
        "Accept-Encoding: gzip, deflate\r\n\r\n"
        "2\r\n"
        "qq\r\n";

    size_t len;
    auto chunk_header_cb = [&](size_t length) {
        len = length;
    };

    parser.bind_cb(parser_t::CHUNK_HEADER, chunk_header_cb);
    auto rv = strlen(data) == parser.execute(data, strlen(data));

    EXPECT_TRUE(rv);
    EXPECT_EQ(len, 2);
}

TEST(Parser, ChunkCompleteAcquire) {
    parser_t parser(parser_t::parser_type_t::RESPONSE);

    const char* data =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Transfer-Encoding: chunked\r\n"
        "Accept: */*\r\n"
        "Accept-Encoding: gzip, deflate\r\n\r\n"
        "2\r\n"
        "qq\r\n"
        "3\r\n"
        "jjj\r\n";

    int count {};
    auto chunk_complete_cb = [&]() {
        count++;
    };

    parser.bind_cb(parser_t::CHUNK_COMPLETE, chunk_complete_cb);
    auto rv = strlen(data) == parser.execute(data, strlen(data));

    EXPECT_TRUE(rv);
    EXPECT_EQ(count, 2);
}

TEST(Parser, ChunkDataAcquire) {
    parser_t parser(parser_t::parser_type_t::RESPONSE);

    const char* data =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Transfer-Encoding: chunked\r\n"
        "Accept: */*\r\n"
        "Accept-Encoding: gzip, deflate\r\n\r\n"
        "2\r\n"
        "qq\r\n"
        "3\r\n"
        "jjj\r\n";

    int count {};
    std::string second_body;
    auto chunk_data_cb = [&](const char* at, size_t length) {
        second_body.reserve(length);
        second_body.assign(at, length);
        count++;
    };

    parser.bind_cb(parser_t::BODY, chunk_data_cb);
    auto rv = strlen(data) == parser.execute(data, strlen(data));

    EXPECT_TRUE(rv);
    EXPECT_EQ(second_body, "jjj");
    EXPECT_EQ(count, 2);
}
