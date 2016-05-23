#include "api.h"
#include "server.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace requests;

TEST(Api, Default) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "http://127.0.0.1:8080/");
    
    EXPECT_EQ(response->http_major().value(), 1);
    EXPECT_EQ(response->http_minor().value(), 1);
    EXPECT_EQ(response->status_code().value(), 200);
    EXPECT_EQ(response->status_message().value(), "OK");
    EXPECT_EQ(response->error().code_to_string(), "SUCCESS");
    EXPECT_EQ(response->redirect_count().value(), 0);
    EXPECT_EQ(response->redirects().size(), 0);
    auto headers = response->headers();
    EXPECT_EQ(headers.at("Content-Type"), "text/html; charset=UTF-8");

    server.stop();
    thread.join();
}

TEST(Api, Ip) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "127.0.0.1:8080/ip");
    
    EXPECT_EQ(response->raw().value(), "127.0.0.1");

    server.stop();
    thread.join();
}

TEST(Api, NonExistsPath) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "127.0.0.1:8080/abracadabra");

    EXPECT_EQ(response->status_code().value(), 404);
    EXPECT_FALSE(response->error());
    EXPECT_EQ(response->raw().value().substr(0, 9), "<!DOCTYPE");

    server.stop();
    thread.join();
}

TEST(Api, UserAgent) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});

    service_t service;
    auto headers = "User-Agent: requests"_headers;
    auto response = Get(service, "127.0.0.1:8080/user-agent", headers);

    EXPECT_EQ(response->status_code().value(), 200);
    EXPECT_FALSE(response->error());
    EXPECT_EQ(response->raw().value(), "user-agent: requests");

    server.stop();
    thread.join();
}

TEST(Api, GetRequestData) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});

    service_t service;
     auto response = Get(service, "127.0.0.1:8080/get?a=1&b=2");

    EXPECT_EQ(response->status_code().value(), 200);
    EXPECT_FALSE(response->error());
    EXPECT_EQ(response->raw().value(),
              "domain: 127.0.0.1\n"
              "path: /get\n"
              "query: a=1&b=2");

    server.stop();
    thread.join();
}

TEST(Api, GzipData) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});

    service_t service;
    auto response = Get(service, "127.0.0.1:8080/gzip");

    EXPECT_EQ(response->status_code().value(), 200);
    EXPECT_FALSE(response->error());
    EXPECT_EQ(response->raw().value().substr(0, 3), "\x1F\x8B\b");
    EXPECT_EQ(decompress(response->raw().value()), "hello world");

    server.stop();
    thread.join();
}

TEST(Api, RedirectsNTimes) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "127.0.0.1:8080/redirect/5");
    
    EXPECT_EQ(response->status_code().value(), 200);
    EXPECT_FALSE(response->error());
    EXPECT_EQ(response->error().code_to_string(), "SUCCESS");
    EXPECT_EQ(response->redirect_count().value(), 5);

    server.stop();
    thread.join();
}

TEST(Api, BasicAuth) {
    server_t server{"127.0.0.1", "4433", true};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto auth = "my_user:my_passwd"_auth;
    auto response = Get(service, "https://127.0.0.1:4433/basic_auth/my_user/my_passwd", auth);
    
    EXPECT_EQ(response->status_code().value(), 200);
    EXPECT_FALSE(response->error());
    EXPECT_EQ(response->content().value(),
              "authenticated: true\n"
              "user: my_user\n"
              "password: my_passwd");

    server.stop();
    thread.join();
}

TEST(Api, Timeout) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "127.0.0.1:8080/delay/1", timeout_t{0});
    
    EXPECT_TRUE(response->error());
    EXPECT_EQ(response->error().code_to_string(), "TIMEOUT");

    server.stop();
    thread.join();
}

TEST(Api, Query) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto params = "123=abc&qwe=!@#$%^*()/;-=+.,<>?"_params;
    auto response = Get(service, "127.0.0.1:8080/params", params);
    
    EXPECT_FALSE(response->error());
    EXPECT_EQ(response->error().code_to_string(), "SUCCESS");
    EXPECT_EQ(response->content().value(), "query: qwe=%2521%2540%2523%2524%2525%255E%252A%2528%2529%252F%253B-%253D%252B.%252C%253C%253E%253F&123=abc");

    server.stop();
    thread.join();
}

TEST(Api, Cookies) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "127.0.0.1:8080/cookies");
    
    EXPECT_FALSE(response->error());
    EXPECT_EQ(response->error().code_to_string(), "SUCCESS");
    EXPECT_EQ(response->cookies().to_string(),
              "cookie1; Expires=Wed, 09 Jun 2021 10:18:14 GMT; HttpOnly\n"
              "cookie2\n\n");
    EXPECT_EQ(response->headers().to_string(),
              "Connection: close\r\n"
              "Server: requests-server\r\n"
              "Content-Type: text/html; charset=UTF-8\r\n"
              "Set-Cookie: cookie1; Expires=Wed, 09 Jun 2021 10:18:14 GMT; HttpOnly\r\n"
              "Set-Cookie: cookie2\r\n\r\n");
    EXPECT_EQ(response->cookies().get("127.0.0.1", "/cookies").to_string(),
              "cookie1; cookie2; ");
    EXPECT_EQ(response->cookies().get("127.0.0.1", "/").to_string(), "");

    server.stop();
    thread.join();
}

TEST(Api, Session) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto session = service.new_session();
    set_option(session, "127.0.0.1:8080/cookies", keep_alive_t{true});
    auto response = session->Get();
    
    EXPECT_EQ(response->http_major().value(), 1);
    EXPECT_EQ(response->http_minor().value(), 1);
    EXPECT_EQ(response->status_code().value(), 200);
    EXPECT_EQ(response->status_message().value(), "OK");
    EXPECT_EQ(response->error().code_to_string(), "SUCCESS");
    EXPECT_EQ(response->redirect_count().value(), 0);
    EXPECT_EQ(response->redirects().size(), 0);
    EXPECT_EQ(response->headers().at("Content-Type"), "text/html; charset=UTF-8");
    EXPECT_EQ(response->request().make_request(),
              "GET /cookies HTTP/1.1\r\n"
              "Host: 127.0.0.1\r\n"
              "Accept: */*\r\n"
              "Accept-Encoding: gzip, deflate\r\n"
              "User-Agent: Mozilla/5.0 (X11; Linux x86_64) "
              "AppleWebKit/537.36 (KHTML, like Gecko) "
              "Chrome/47.0.2526.106 Safari/537.36\r\n"
              "Connection: keep-alive\r\n\r\n");
    EXPECT_EQ(response->request().cookies().to_string(), "");
    EXPECT_EQ(response->cookies().to_string(),
              "cookie1; Expires=Wed, 09 Jun 2021 10:18:14 GMT; HttpOnly\n"
              "cookie2\n\n");

    
    response = session->Get();

    
    EXPECT_EQ(response->http_major().value(), 1);
    EXPECT_EQ(response->http_minor().value(), 1);
    EXPECT_EQ(response->status_code().value(), 200);
    EXPECT_EQ(response->status_message().value(), "OK");
    EXPECT_EQ(response->error().code_to_string(), "SUCCESS");
    EXPECT_EQ(response->error().to_string(), "SUCCESS: success");
    EXPECT_EQ(response->redirect_count().value(), 0);
    EXPECT_EQ(response->redirects().size(), 0);
    EXPECT_EQ(response->headers().at("Content-Type"), "text/html; charset=UTF-8");
    EXPECT_EQ(response->request().make_request(),
              "GET /cookies HTTP/1.1\r\n"
              "Cookies: cookie1; cookie2; \r\n"
              "Host: 127.0.0.1\r\n"
              "Accept: */*\r\n"
              "Accept-Encoding: gzip, deflate\r\n"
              "User-Agent: Mozilla/5.0 (X11; Linux x86_64) "
              "AppleWebKit/537.36 (KHTML, like Gecko) "
              "Chrome/47.0.2526.106 Safari/537.36\r\n"
              "Connection: keep-alive\r\n\r\n");
    EXPECT_EQ(response->request().cookies().to_string(),
              "cookie1; Expires=Wed, 09 Jun 2021 10:18:14 GMT; HttpOnly\n"
              "cookie2\n\n");
    EXPECT_EQ(response->cookies().to_string(),
              "cookie1; Expires=Wed, 09 Jun 2021 10:18:14 GMT; HttpOnly\n"
              "cookie2\n\n");

    server.stop();
    thread.join();
}

TEST(Api, SessionWithDifferentArguments) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto session = service.new_session("127.0.0.1:8080/", keep_alive_t{true});
    auto response = session->Get();
    
    EXPECT_EQ(response->http_major().value(), 1);
    EXPECT_EQ(response->http_minor().value(), 1);
    EXPECT_EQ(response->status_code().value(), 200);
    EXPECT_EQ(response->status_message().value(), "OK");
    EXPECT_EQ(response->error().code_to_string(), "SUCCESS");
    EXPECT_EQ(response->redirect_count().value(), 0);
    EXPECT_EQ(response->redirects().size(), 0);
    EXPECT_EQ(response->headers().at("Content-Type"), "text/html; charset=UTF-8");

    server.stop();
    thread.join();
}

TEST(Api, AsyncDefault) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = AsyncGet(service, "http://127.0.0.1:8080/").get();
    
    EXPECT_EQ(response->http_major().value(), 1);
    EXPECT_EQ(response->http_minor().value(), 1);
    EXPECT_EQ(response->status_code().value(), 200);
    EXPECT_EQ(response->status_message().value(), "OK");
    EXPECT_EQ(response->error().code_to_string(), "SUCCESS");
    EXPECT_EQ(response->redirect_count().value(), 0);
    EXPECT_EQ(response->redirects().size(), 0);
    auto headers = response->headers();
    EXPECT_EQ(headers.at("Content-Type"), "text/html; charset=UTF-8");

    server.stop();
    thread.join();
}

TEST(Api, SessionAsyncGet) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto session = service.new_session("127.0.0.1:8080/", keep_alive_t{true});
    auto response = session->AsyncGet().get();
    
    EXPECT_EQ(response->http_major().value(), 1);
    EXPECT_EQ(response->http_minor().value(), 1);
    EXPECT_EQ(response->status_code().value(), 200);
    EXPECT_EQ(response->status_message().value(), "OK");
    EXPECT_EQ(response->error().code_to_string(), "SUCCESS");
    EXPECT_EQ(response->redirect_count().value(), 0);
    EXPECT_EQ(response->redirects().size(), 0);
    EXPECT_EQ(response->headers().at("Content-Type"), "text/html; charset=UTF-8");

    server.stop();
    thread.join();
}

TEST(Api, SessionAsyncGetTwoRequests) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto session = service.new_session( "127.0.0.1:8080/", timeout_t{0});
    
    auto response_future = session->AsyncGet();
    auto response = response_future.get();
    
    EXPECT_EQ(response->error().code_to_string(), "TIMEOUT");

    set_option(session, timeout_t{5});
    auto response2_future = session->AsyncGet();
    response = response2_future.get();
    
    EXPECT_EQ(response->error().code_to_string(), "SUCCESS");

    server.stop();
    thread.join();
}
