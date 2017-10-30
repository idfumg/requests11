#include "uri.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace crequests;

TEST(Uri, Protocol) {
    const string_t url {"http://"};
    const auto uri = uri_t::from_string(url);

    EXPECT_TRUE(not uri.is_valid());
}

TEST(Uri, ProtocolWithoutSlashes) {
    const string_t url {"some_protocol:google.ru:80/"};
    const auto uri = uri_t::from_string(url);

    EXPECT_TRUE(not uri.is_valid());
}

TEST(Uri, Domain) {
    const string_t url {"google.com"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "google.com"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
}

TEST(Uri, ProtocolAndDomain) {
    const string_t url {"http://google.com"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "http://google.com"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
}

TEST(Uri, DomainAndPort) {
    const string_t url {"google.com:80"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "google.com:80"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
}

TEST(Uri, ProtocolAndDomainAndPort) {
    const string_t url {"http://google.com:80"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "http://google.com:80"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
}

TEST(Uri, DomainAndPath) {
    const string_t url {"google.com/"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "google.com/"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.path(), "/"_path);
}

TEST(Uri, DomainAndPortAndPath) {
    const string_t url {"google.com:80/"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "google.com:80/"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/"_path);
}

TEST(Uri, ProtocolAndDomainAndPortAndPath) {
    const string_t url {"http://google.com:80/"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "http://google.com:80/"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/"_path);
}

TEST(Uri, DomainAndPathAndFragment) {
    const string_t url {"google.com/#fragment"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "google.com/#fragment"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.path(), "/"_path);
    EXPECT_EQ(uri.fragment(), "fragment"_fragment);
}

TEST(Uri, DomainAndPortAndPathAndFragment) {
    const string_t url {"google.com:80/asd/#fragment"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "google.com:80/asd/#fragment"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/asd/"_path);
    EXPECT_EQ(uri.fragment(), "fragment"_fragment);
}

TEST(Uri, ProtocolAndDomainAndPortAndPathAndFragment) {
    const string_t url {"http://google.com:80/asd/#fragment"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "http://google.com:80/asd/#fragment"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/asd/"_path);
    EXPECT_EQ(uri.fragment(), "fragment"_fragment);
}

TEST(Uri, ProtocolAndDomainAndPortAndPathAndFragmentAndQuery) {
    const string_t url {"http://google.com:80/asd/?a=1&b=2#fragment"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "http://google.com:80/asd/?a=1&b=2#fragment"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/asd/"_path);
    EXPECT_EQ(uri.fragment(), "fragment"_fragment);
    EXPECT_EQ(uri.query(), "a=1&b=2"_query);
}

TEST(Uri, ProtocolAndDomainAndPortAndPathAndQuery) {
    const string_t url {"http://google.com:80/asd/?a=1&b=2"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "http://google.com:80/asd/?a=1&b=2"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/asd/"_path);
    EXPECT_EQ(uri.query(), "a=1&b=2"_query);
}

TEST(Uri, ProtocolAndDomainAndPortAndQuery) {
    const string_t url {"http://google.com:80?a=1&b=2"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "http://google.com:80?a=1&b=2"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.query(), "a=1&b=2"_query);
}

TEST(Uri, ProtocolAndDomainAndQuery) {
    const string_t url {"http://google.com?a=1&b=2"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "http://google.com?a=1&b=2"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.query(), "a=1&b=2"_query);
}

TEST(Uri, DomainAndQuery) {
    const string_t url {"google.com?a=1&b=2"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "google.com?a=1&b=2"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.query(), "a=1&b=2"_query);
}

TEST(Uri, DomainAndPortAndQuery) {
    const string_t url {"google.com:80?a=1&b=2"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "google.com:80?a=1&b=2"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.query(), "a=1&b=2"_query);
}

TEST(Uri, DomainAndPathAndQuery) {
    const string_t url {"google.com/?a=1&b=2"};
    const auto uri = uri_t::from_string(url);

    EXPECT_EQ(uri.url(), "google.com/?a=1&b=2"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.path(), "/"_path);
    EXPECT_EQ(uri.query(), "a=1&b=2"_query);
}

TEST(Uri, UsingUserDefinedLiteral) {
    auto uri = "http://google.com:80/?a=1"_uri;
    uri.prepare();

    EXPECT_TRUE(uri.is_valid());
    EXPECT_EQ(uri.url(), "http://google.com:80/?a=1"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/"_path);
    EXPECT_EQ(uri.query(), "a=1"_query);
}

TEST(UriPrepare, ParametersFromUrlOverrideCurrent) {
    const string_t url {"http://google.com:80/asd/?a=1&b=2#fragment"};
    auto uri = uri_t::from_string(url);

    uri.protocol("https"_protocol);
    uri.domain("goooogle.com"_domain);
    uri.port("443"_port);
    uri.path("/qweq/weqweqwe/qweqwe"_path);
    uri.fragment("asdaqweqwe123123123"_fragment);
    uri.query("123123=a&qweqwe=bbbb"_query);

    uri.prepare();

    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/asd/"_path);
    EXPECT_EQ(uri.fragment(), "fragment"_fragment);
    EXPECT_EQ(uri.params(), "b=2&a=1&123123=a&qweqwe=bbbb"_params);
}

TEST(UriPrepare, ParamsOverrideAndAppendedToQueryParams) {
    auto uri = uri_t::from_string("http://google.com?a=1&b=2");

    const params_t params = {
        { "a", "1" },
        { "c", "3" },
        { "d", "4" }
    };

    uri.params(params);
    uri.prepare();

    EXPECT_EQ(uri.params(), "c=3&d=4&a=1&b=2"_params);
}

TEST(UriPrepare, DefaultHttpProtocolAndPort) {
    const string_t url {"google.com"};
    auto uri = uri_t::from_string(url);

    uri.prepare();

    EXPECT_EQ(uri.url(), "http://google.com:80/"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.port(), "80"_port);
}

TEST(UriPrepare, DefaultSslProtocolFromSslPort) {
    const string_t url {"google.com"};
    auto uri = uri_t::from_string(url);

    uri.port("443"_port);
    uri.prepare();

    EXPECT_EQ(uri.url(), "https://google.com:443/"_url);
    EXPECT_EQ(uri.protocol(), "https"_protocol);
    EXPECT_EQ(uri.port(), "443"_port);
}

TEST(UriPrepare, DefaultSslPortFromSslProtocol) {
    const string_t url {"google.com"};
    auto uri = uri_t::from_string(url);

    uri.protocol("https"_protocol);
    uri.prepare();

    EXPECT_EQ(uri.url(), "https://google.com:443/"_url);
    EXPECT_EQ(uri.protocol(), "https"_protocol);
    EXPECT_EQ(uri.port(), "443"_port);
}

TEST(UriPrepare, QueryWithSlashes) {
    const string_t url {"127.0.0.1/response-headers?Content-Type=text/plain"};
    auto uri = uri_t::from_string(url);
    uri.prepare();

    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "127.0.0.1"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/response-headers"_path);
    EXPECT_EQ(uri.query(), "Content-Type=text%2Fplain"_query);
}
