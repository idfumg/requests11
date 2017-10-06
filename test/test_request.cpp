#include "request.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace crequests;

TEST(Request, PrepareDefault) {
    request_t request;
    request.domain("google.com"_domain);
    request.url("google.com"_url);
    request.gzip(gzip_t{false});
    request.prepare();
    std::ostringstream out;
    out << request.make_request();
    
    EXPECT_EQ(out.str(),
              "GET / HTTP/1.1\r\n"
              "Host: google.com\r\n"
              "Accept-Encoding: gzip, deflate\r\n"
              "Connection: keep-alive\r\n"
              "Accept: */*\r\n"
              "User-Agent: Mozilla/5.0 (X11; Linux x86_64) "
                          "AppleWebKit/537.36 (KHTML, like Gecko) "
                          "Chrome/47.0.2526.106 Safari/537.36\r\n\r\n");
}

TEST(Request, PreparePost) {
    request_t request;
    request.domain("google.com"_domain);
    request.url("google.com"_url);
    request.method("POST"_method);
    request.gzip(gzip_t{false});
    request.prepare();
    std::ostringstream out;
    out << request.make_request();
    
    EXPECT_EQ(out.str(),
              "POST / HTTP/1.1\r\n"
              "Host: google.com\r\n"
              "Accept-Encoding: gzip, deflate\r\n"
              "Connection: keep-alive\r\n"
              "Accept: */*\r\n"
              "User-Agent: Mozilla/5.0 (X11; Linux x86_64) "
                          "AppleWebKit/537.36 (KHTML, like Gecko) "
                          "Chrome/47.0.2526.106 Safari/537.36\r\n\r\n");
}

TEST(Request, PrepareGzip) {
    request_t request;
    request.domain("google.com"_domain);
    request.url("google.com"_url);
    request.method("POST"_method);
    request.gzip(gzip_t{true});
    request.prepare();
    std::ostringstream out;
    out << request.make_request();
    
    EXPECT_EQ(out.str(),
              "POST / HTTP/1.1\r\n"
              "Content-Encoding: gzip\r\n"
              "Host: google.com\r\n"
              "Accept-Encoding: gzip, deflate\r\n"
              "Connection: keep-alive\r\n"
              "Accept: */*\r\n"
              "User-Agent: Mozilla/5.0 (X11; Linux x86_64) "
                          "AppleWebKit/537.36 (KHTML, like Gecko) "
                          "Chrome/47.0.2526.106 Safari/537.36\r\n\r\n");
}

TEST(Request, PrepareKeepAlive) {
    request_t request;
    request.domain("google.com"_domain);
    request.url("google.com"_url);
    request.method("POST"_method);
    request.keep_alive(keep_alive_t{false});
    request.gzip(gzip_t{false});
    request.prepare();
    std::ostringstream out;
    out << request.make_request();
    
    EXPECT_EQ(out.str(),
              "POST / HTTP/1.1\r\n"
              "Host: google.com\r\n"
              "Accept-Encoding: gzip, deflate\r\n"
              "Connection: close\r\n"
              "Accept: */*\r\n"
              "User-Agent: Mozilla/5.0 (X11; Linux x86_64) "
                          "AppleWebKit/537.36 (KHTML, like Gecko) "
                          "Chrome/47.0.2526.106 Safari/537.36\r\n\r\n");
}

TEST(Request, PrepareBasicAuthorization) {
    request_t request;
    request.domain("google.com"_domain);
    request.url("google.com"_url);
    request.method("POST"_method);
    request.auth(auth_t{login_t{"user"}, password_t{"passwd"}});
    request.gzip(gzip_t{false});
    request.prepare();
    std::ostringstream out;
    out << request.make_request();
    
    EXPECT_EQ(out.str(),
              "POST / HTTP/1.1\r\n"
              "Authorization: Basic dXNlcjpwYXNzd2Q=\r\n"
              "Host: google.com\r\n"
              "Accept-Encoding: gzip, deflate\r\n"
              "Connection: keep-alive\r\n"
              "Accept: */*\r\n"
              "User-Agent: Mozilla/5.0 (X11; Linux x86_64) "
                          "AppleWebKit/537.36 (KHTML, like Gecko) "
                          "Chrome/47.0.2526.106 Safari/537.36\r\n\r\n"
              );
}

TEST(Request, PrepareWithData) {
    request_t request;
    request.domain("google.com"_domain);
    request.url("google.com"_url);
    request.method("POST"_method);
    request.data("hellow"_data);
    request.gzip(gzip_t{false});
    request.prepare();
    std::ostringstream out;
    out << request.make_request();
    
    EXPECT_EQ(out.str(),
              "POST / HTTP/1.1\r\n"
              "Content-Length: 6\r\n"
              "Host: google.com\r\n"
              "Accept-Encoding: gzip, deflate\r\n"
              "Connection: keep-alive\r\n"
              "Accept: */*\r\n"
              "User-Agent: Mozilla/5.0 (X11; Linux x86_64) "
                          "AppleWebKit/537.36 (KHTML, like Gecko) "
                          "Chrome/47.0.2526.106 Safari/537.36\r\n\r\n"
              "hellow");
    
    EXPECT_FALSE(request.is_ssl());
}

TEST(Request, PrepareWithGzippedData) {
    request_t request;
    request.url("https://google.com"_url);
    request.method("POST"_method);
    request.data("hellow"_data);
    request.gzip(gzip_t{true});
    request.prepare();
    std::ostringstream out;
    out << request.make_request();
    
    EXPECT_EQ(out.str().substr(0, 271),
              "POST / HTTP/1.1\r\n"
              "Content-Length: 6\r\n"
              "Content-Encoding: gzip\r\n"
              "Host: google.com\r\n"
              "Accept-Encoding: gzip, deflate\r\n"
              "Connection: keep-alive\r\n"
              "Accept: */*\r\n"
              "User-Agent: Mozilla/5.0 (X11; Linux x86_64) "
                          "AppleWebKit/537.36 (KHTML, like Gecko) "
                          "Chrome/47.0.2526.106 Safari/537.36\r\n\r\n"
              "\x1F\x8B\b");

    EXPECT_TRUE(request.is_ssl());
}

TEST(Request, UsingUserDefinedLiteral) {
    request_t request;
    request.uri("https://google.com/"_uri);
    request.prepare();

    EXPECT_EQ(request.make_request(),
              "GET / HTTP/1.1\r\n"
              "Content-Encoding: gzip\r\n"
              "Host: google.com\r\n"
              "Accept-Encoding: gzip, deflate\r\n"
              "Connection: keep-alive\r\n"
              "Accept: */*\r\n"
              "User-Agent: Mozilla/5.0 (X11; Linux x86_64) "
                          "AppleWebKit/537.36 (KHTML, like Gecko) "
                          "Chrome/47.0.2526.106 Safari/537.36\r\n"
              "\r\n");
}
