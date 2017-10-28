#include "headers.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace crequests;

TEST(Headers, FromString) {
    const auto headers =
        headers_t::from_string("a: 1\r\nb: 2\r\nc:3\r\nd:  4 \r\n\r\n");

    EXPECT_EQ(headers,
              (headers_t {{"a", "1"}, {"b", "2"}, {"c", "3"}, {"d", "4"}}));
}

TEST(Headers, FromStringUsingUserDefinedLiteral) {
    const auto headers =
        "a: 1\r\nb: 2\r\nc:3\r\nd:  4 \r\n\r\n"_headers;

    EXPECT_EQ(headers,
              (headers_t {{"a", "1"}, {"b", "2"}, {"c", "3"}, {"d", "4"}}));
}

TEST(Headers, ToString) {
    const auto headers =
        headers_t {{"a", "1"}, {"d", "4"}, {"b", "2"}, {"c", "3"}};

    EXPECT_EQ(headers.to_string(), "a: 1\r\nb: 2\r\nc: 3\r\nd: 4\r\n\r\n");
}

TEST(Headers, Update) {
    auto headers1 = headers_t {{"a", "1"}, {"b", "2"}};
    const auto headers2 = headers_t {{"c", "3"}, {"d", "4"}};
    headers1.update(headers2);

    const auto result = headers_t {{"c", "3"}, {"d", "4"}, {"a", "1"}, {"b", "2"}};

    EXPECT_EQ(headers1, result);
}
