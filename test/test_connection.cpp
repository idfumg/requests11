#include "api.h"
#include "server.h"
#include "gtest/gtest.h"

#include <thread>

using namespace testing;
using namespace crequests;

TEST(ConnectionBad,  ResolveError) {
    service_t service;
    auto response = Get(service, "nonexistshost.9").get();

    EXPECT_EQ(response->error().code(), error_code_t::RESOLVE_ERROR);
}

TEST(ConnectionBad,  ConnectError) {
    service_t service;
    auto response = Get(service, "127.0.0.1:8088").get();

    EXPECT_EQ(response->error().code(), error_code_t::CONNECT_ERROR);
}

TEST(ConnectionBad,  ServerDoesNotReadHeadersAndCloseSocket) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "http://127.0.0.1:8080/read_status_error").get();

    EXPECT_EQ(response->error().code(), error_code_t::READ_STATUS_ERROR);

    server.stop();
    thread.join();
}

TEST(ConnectionBad,  ServerReadHeadersAndCloseSocket) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "127.0.0.1:8080/get_wrong_status").get();

    EXPECT_EQ(response->error().code(), error_code_t::READ_STATUS_ERROR);

    server.stop();
    thread.join();
}

TEST(ConnectionBad,  ReadStatusDataError) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "127.0.0.1:8080/get_wrong_status_data").get();

    EXPECT_EQ(response->error().code(), error_code_t::READ_STATUS_DATA_ERROR);

    server.stop();
    thread.join();
}

TEST(ConnectionBad,  ReadHeadersError) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "127.0.0.1:8080/get_wrong_headers").get();

    EXPECT_EQ(response->error().code(), error_code_t::READ_HEADERS_ERROR);

    server.stop();
    thread.join();
}

TEST(ConnectionBad,  ContentLengthError) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "127.0.0.1:8080/get_wrong_content").get();

    EXPECT_EQ(response->error().code(), error_code_t::READ_CONTENT_LENGTH_ERROR);

    server.stop();
    thread.join();
}

TEST(ConnectionBad,  ChunkDataError) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "127.0.0.1:8080/get_wrong_chunk_data").get();

    EXPECT_EQ(response->error().code(), error_code_t::READ_CHUNK_DATA_ERROR);

    server.stop();
    thread.join();
}

TEST(ConnectionGood,  ReadHeadersNoDataExists) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "127.0.0.1:8080").get();

    EXPECT_EQ(response->error().code(), error_code_t::SUCCESS);
    EXPECT_EQ(
        response->headers().to_string(),
        "Connection: close\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Server: requests-server\r\n\r\n");

    server.stop();
    thread.join();
}

TEST(ConnectionGood,  ReadHeadersWithData) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "127.0.0.1:8080/get_response_with_data").get();

    EXPECT_EQ(response->error().code(), error_code_t::SUCCESS);
    EXPECT_EQ(
        response->headers().to_string(),
        "Connection: close\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Server: requests-server\r\n"
        "Content-Length: 9\r\n\r\n");

    server.stop();
    thread.join();
}

TEST(ConnectionGood,  GetContentLength) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "127.0.0.1:8080/get_content_length").get();

    EXPECT_EQ(response->error().code(), error_code_t::SUCCESS);
    EXPECT_EQ(response->raw().value().front(), 'a');
    EXPECT_EQ(response->raw().value().back(), 'z');
    EXPECT_EQ(response->raw().value().size(), 100);

    server.stop();
    thread.join();
}

TEST(ConnectionGood,  GetBigContentLength) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "127.0.0.1:8080/get_big_content_length").get();

    EXPECT_EQ(response->error().code(), error_code_t::SUCCESS);
    EXPECT_EQ(response->raw().value().front(), 'a');
    EXPECT_EQ(response->raw().value().back(), 'z');
    EXPECT_EQ(response->raw().value().size(), 10000);

    server.stop();
    thread.join();
}

TEST(ConnectionGood,  GetBigChunks) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "127.0.0.1:8080/get_big_chunks").get();

    EXPECT_EQ(response->error().code(), error_code_t::SUCCESS);
    EXPECT_EQ(response->raw().value(), string_t(1500, 's'));

    server.stop();
    thread.join();
}

TEST(ConnectionGood,  GetBigUntilEof) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "127.0.0.1:8080/get_big_until_eof").get();

    EXPECT_EQ(response->error().code(), error_code_t::SUCCESS);
    EXPECT_EQ(response->raw().value().size(), 10000);

    server.stop();
    thread.join();
}

TEST(ConnectionGood,  GetBigUntilEofSsl) {
    server_t server{"127.0.0.1", "4433", true};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    auto response = Get(service, "https://127.0.0.1:4433/get_big_until_eof").get();

    EXPECT_EQ(response->error().code(), error_code_t::SUCCESS);
    EXPECT_EQ(response->raw().value().size(), 10000);

    server.stop();
    thread.join();
}

TEST(ConnectionBad, ThrowOnError) {
    server_t server{"127.0.0.1", "8080"};
    std::thread thread([&server](){server.run();});
    
    service_t service;
    try {
        auto response =
            Get(service, "http://127.0.0.1:8080/read_status_error",
                throw_on_error_t{true}).get();
    } catch (const crequests::error_t& e) {
        EXPECT_EQ(e.code(), error_code_t::READ_STATUS_ERROR);
    }

    server.stop();
    thread.join();
}
