#include "uri.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace requests;

TEST(Uri, Protocol) {
    string_t url {"http://"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "http://"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
}

TEST(Uri, ProtocolWithoutSlashes) {
    string_t url {"some_protocol:google.ru:80/"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "some_protocol:google.ru:80/"_url);
    EXPECT_EQ(uri.protocol(), "some_protocol"_protocol);
    EXPECT_EQ(uri.domain(), "google.ru"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/"_path);
}

TEST(Uri, Domain) {
    string_t url {"google.com"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "google.com"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
}

TEST(Uri, ProtocolAndDomain) {
    string_t url {"http://google.com"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "http://google.com"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
}

TEST(Uri, DomainAndPort) {
    string_t url {"google.com:80"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "google.com:80"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
}

TEST(Uri, ProtocolAndDomainAndPort) {
    string_t url {"http://google.com:80"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "http://google.com:80"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
}

TEST(Uri, DomainAndPath) {
    string_t url {"google.com/"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "google.com/"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.path(), "/"_path);
}

TEST(Uri, DomainAndPortAndPath) {
    string_t url {"google.com:80/"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "google.com:80/"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/"_path);
}

TEST(Uri, ProtocolAndDomainAndPortAndPath) {
    string_t url {"http://google.com:80/"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "http://google.com:80/"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/"_path);
}

TEST(Uri, DomainAndPathAndFragment) {
    string_t url {"google.com/#fragment"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "google.com/#fragment"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.path(), "/"_path);
    EXPECT_EQ(uri.fragment(), "fragment"_fragment);
}

TEST(Uri, DomainAndPortAndPathAndFragment) {
    string_t url {"google.com:80/asd/#fragment"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "google.com:80/asd/#fragment"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/asd/"_path);
    EXPECT_EQ(uri.fragment(), "fragment"_fragment);
}

TEST(Uri, ProtocolAndDomainAndPortAndPathAndFragment) {
    string_t url {"http://google.com:80/asd/#fragment"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "http://google.com:80/asd/#fragment"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/asd/"_path);
    EXPECT_EQ(uri.fragment(), "fragment"_fragment);
}

TEST(Uri, ProtocolAndDomainAndPortAndPathAndFragmentAndQuery) {
    string_t url {"http://google.com:80/asd/#fragment?a=1&b=2"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "http://google.com:80/asd/#fragment?a=1&b=2"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/asd/"_path);
    EXPECT_EQ(uri.fragment(), "fragment"_fragment);
    EXPECT_EQ(uri.query(), "a=1&b=2"_query);
}

TEST(Uri, ProtocolAndDomainAndPortAndPathAndQuery) {
    string_t url {"http://google.com:80/asd/?a=1&b=2"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "http://google.com:80/asd/?a=1&b=2"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/asd/"_path);
    EXPECT_EQ(uri.query(), "a=1&b=2"_query);
}

TEST(Uri, ProtocolAndDomainAndPortAndQuery) {
    string_t url {"http://google.com:80?a=1&b=2"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "http://google.com:80?a=1&b=2"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.query(), "a=1&b=2"_query);
}

TEST(Uri, ProtocolAndDomainAndQuery) {
    string_t url {"http://google.com?a=1&b=2"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "http://google.com?a=1&b=2"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.query(), "a=1&b=2"_query);
}

TEST(Uri, DomainAndQuery) {
    string_t url {"google.com?a=1&b=2"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "google.com?a=1&b=2"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.query(), "a=1&b=2"_query);
}

TEST(Uri, DomainAndPortAndQuery) {
    string_t url {"google.com:80?a=1&b=2"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "google.com:80?a=1&b=2"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.query(), "a=1&b=2"_query);
}

TEST(Uri, DomainAndPathAndQuery) {
    string_t url {"google.com/?a=1&b=2"};
    auto uri = uri_t::from_string(url);
    
    EXPECT_EQ(uri.url(), "google.com/?a=1&b=2"_url);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.path(), "/"_path);
    EXPECT_EQ(uri.query(), "a=1&b=2"_query);
}

TEST(Uri, UsingUserDefinedLiteral) {
    auto uri = "http://google.com:80/?a=1"_uri;
    
    EXPECT_EQ(uri.url(), "http://google.com:80/?a=1"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/"_path);
    EXPECT_EQ(uri.query(), "a=1"_query);
}

TEST(UriPrepare, ProtocolWithoutUrl) {
    uri_t uri;
    uri.protocol("https"_protocol);
    uri.prepare();
    
    EXPECT_EQ(uri.protocol(), "https"_protocol);
}

TEST(UriPrepare, ProtocolWithUrl) {
    uri_t uri = uri_t::from_string("http://");
    uri.protocol(protocol_t{"https"});
    uri.prepare();
    
    EXPECT_EQ(uri.protocol(), "http"_protocol);
}

TEST(UriPrepare, ParametersFromUrlOverrideCurrent) {
    string_t url {"http://google.com:80/asd/#fragment?a=1&b=2"};
    auto uri = uri_t::from_string(url);

    uri.protocol("https"_protocol);
    uri.domain("goooogle.com"_domain);
    uri.port("443"_port);
    uri.path("/qweq/weqweqwe/qweqwe"_path);
    uri.fragment("asdaqweqwe123123123"_fragment);
    uri.query("123123=a&qweqwe=bbbb"_query);

    uri.prepare();
    
    EXPECT_EQ(uri.url(),
              "http://google.com:80/asd/#fragment"
              "?b=2&a=1&qweqwe=bbbb&123123=a"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "google.com"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/asd/"_path);
    EXPECT_EQ(uri.fragment(), "fragment"_fragment);
    EXPECT_EQ(uri.query(), "b=2&a=1&qweqwe=bbbb&123123=a"_query);
}

TEST(UriPrepare, ParamsOverrideAndAppendedToQueryParams) {
    string_t url {"http://google.com?a=1&b=2"};
    auto uri = uri_t::from_string(url);

    params_t params = {
        { "a", "1" },
        { "c", "3" },
        { "d", "4" }
    };
    
    uri.params(params);
    uri.prepare();
    
    EXPECT_EQ(uri.url(), "http://google.com:80/?c=3&d=4&b=2&a=1"_url);
    EXPECT_EQ(uri.query(), "c=3&d=4&b=2&a=1"_query);
}

TEST(UriPrepare, DefaultHttpProtocolAndPort) {
    string_t url {"google.com"};
    auto uri = uri_t::from_string(url);

    uri.prepare();

    EXPECT_EQ(uri.url(), "http://google.com:80/"_url);
    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.port(), "80"_port);
}

TEST(UriPrepare, DefaultSslProtocolFromSslPort) {
    string_t url {"google.com"};
    auto uri = uri_t::from_string(url);

    uri.port("443"_port);
    uri.prepare();

    EXPECT_EQ(uri.url(), "https://google.com:443/"_url);
    EXPECT_EQ(uri.protocol(), "https"_protocol);
    EXPECT_EQ(uri.port(), "443"_port);
}

TEST(UriPrepare, DefaultSslPortFromSslProtocol) {
    string_t url {"google.com"};
    auto uri = uri_t::from_string(url);

    uri.protocol("https"_protocol);
    uri.prepare();

    EXPECT_EQ(uri.url(), "https://google.com:443/"_url);
    EXPECT_EQ(uri.protocol(), "https"_protocol);
    EXPECT_EQ(uri.port(), "443"_port);
}

TEST(UriPrepare, QueryWithSlashes) {
    string_t url {"127.0.0.1/response-headers?Content-Type=text/plain"};
    auto uri = uri_t::from_string(url);
    uri.prepare();

    EXPECT_EQ(uri.protocol(), "http"_protocol);
    EXPECT_EQ(uri.domain(), "127.0.0.1"_domain);
    EXPECT_EQ(uri.port(), "80"_port);
    EXPECT_EQ(uri.path(), "/response-headers"_path);
    EXPECT_EQ(uri.query(), "Content-Type=text%2Fplain"_query);
}
