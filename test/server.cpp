#include "server.h"

#include "../crequests/stream.h"
#include "../crequests/utils.h"
#include "../crequests/headers.h"
#include "../crequests/request.h"

namespace crequests {

    namespace {

        template <class StreamBufT>
        string_t read_buffer(StreamBufT& response_buf, std::size_t length) {
            if (length == 0)
                return "";
    
            vector_t<char> buf(length);
            response_buf.sgetn(&buf[0], length);

            return string_t(buf.begin(), buf.end());
        }

        template <class StreamBufT>
        headers_t parse_headers(StreamBufT&& response_buf) {
            std::istream response_stream(&response_buf);
            headers_t headers;

            string_t header;
            while (std::getline(response_stream, header) && header != "\r") {
                const auto ind = header.find(":");
                if (ind != string_t::npos)
                    headers.emplace(
                        trim(header.substr(0, ind)),
                        trim(header.substr(ind + 2))
                        );
            }
    
            return headers;
        }
        
    } /* anonymous namespace */

    namespace {

        const headers_t SERVER_DEFAULT_HEADERS {
            {"Content-Type", "text/html; charset=UTF-8"},
            {"Connection", "close"},
            {"Server", "requests-server"}};

        class server_request_t {
        public:
            void prepare() {
                uri.prepare();
            }
        
        public:
            uri_t uri {};
            method_t method {};
            headers_t headers {};
        };

        class server_response_t {
        public:
            void prepare() {
            
            }

            string_t make_http_response() {
                std::ostringstream out;

                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string();

                return out.str();
            }

            string_t get_response_with_data() {
                std::ostringstream out;

                const string_t data = "some_data";
                headers.insert("Content-Length", std::to_string(data.size()));
                
                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string();
                out << data;

                return out.str();
            }

            string_t get_wrong_status_data() const {
                std::ostringstream out;

                out << "HT/1.1 200 OK\r\n";

                return out.str();
            }

            string_t get_wrong_headers() const {
                std::ostringstream out;

                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string().substr(0, 5);
                out << "\r\n\r\n";

                return out.str();
            }

            string_t get_wrong_content() {
                std::ostringstream out;

                headers.insert("Content-Length", "100");
                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string();
                out << "123";

                return out.str();
            }

            string_t get_wrong_chunk_data() {
                std::ostringstream out;

                headers.insert("Transfer-Encoding", "chunked");
                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string();
                out << std::hex << 5;
                out << "\r\n";
                out << std::hex << "1234";

                return out.str();
            }

            string_t get_content_length() {
                std::ostringstream out;

                string_t data(100, 's');
                data.front() = 'a';
                data.back() = 'z';

                headers.insert("Content-Length", std::to_string(data.size()));
                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string();
                out << data;

                return out.str();
            }

            string_t get_big_content_length() {
                std::ostringstream out;

                string_t data(10000, 's');
                data.front() = 'a';
                data.back() = 'z';

                headers.insert("Content-Length", std::to_string(data.size()));
                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string();
                out << data;

                return out.str();
            }

            string_t get_big_chunks() {
                std::ostringstream out;

                string_t data(1500, 's');

                headers.insert("Transfer-Encoding", "chunked");
                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string();
                while (not data.empty()) {
                    auto len = data.size() > 500 ? 500 : data.size();
                    out << std::hex << len << "\r\n";
                    out << data.substr(0, len) << "\r\n";
                    data.replace(0, len, "");
                }

                return out.str();
            }

            string_t get_big_until_eof() {
                std::ostringstream out;

                string_t data(10000, 's');
                data.front() = 'a';
                data.back() = 'z';

                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string();
                out << data;

                return out.str();
            }

            string_t ip(stream_t& stream) {
                std::ostringstream out;

                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string();
                out << stream.socket<tcp_socket_t::lowest_layer_type>().remote_endpoint().address().to_string();

                return out.str();
            }

            string_t _404() {
                std::ostringstream out;

                out << "HTTP/1.1 404 OK\r\n";
                out << headers.to_string();
                out << "<!DOCTYPE html><html><head><title>Not Found</title></head><body><h1>404 Not Found</h1></body></html>";

                return out.str();
            }

            string_t user_agent() {
                std::ostringstream out;

                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string();
                if (request.headers.count("User-Agent"))
                    out << "user-agent: " << request.headers.at("User-Agent");

                return out.str();
            }

            string_t get() {
                std::ostringstream out;

                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string();
                out << "domain: " << request.uri.domain() << "\n"
                    << "path: " << request.uri.path() << "\n"
                    << "query: " << request.uri.query();

                return out.str();
            }

            string_t gzip() {
                std::ostringstream out;

                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string();
                out << compress("hello world");

                return out.str();
            }

            string_t redirect() {
                std::ostringstream out;

                const auto found = request.uri.path().value().find_last_of("/");
                size_t n {};
                try {
                    if (found != string_t::npos)
                        n = std::strtoul(
                            request.uri.path().value().substr(found+1).c_str(), nullptr, 0);
                } catch (...) {
                }

                string_t code = "200 OK";
                if (n > 0) {
                    code = "301 MOVED";
                    headers.insert(
                        "Location",
                        "http://127.0.0.1:8080/redirect/" + std::to_string(--n));
                }
                
                out << "HTTP/1.1 " << code << "\r\n";
                out << headers.to_string();

                return out.str();
            }

            string_t basic_auth() {
                std::ostringstream out;

                auto splitted = split(request.uri.path().value(), '/');
                if (splitted.size() != 4)
                    return _404();

                const auto user = splitted.at(2);
                const auto passwd = splitted.at(3);

                string_t auth = "";
                if (request.headers.count("Authorization"))
                    auth = request.headers.at("Authorization");

                const auto found = auth.find("Basic ");
                if (found == string_t::npos)
                    return _404();

                auth = auth.substr(found + 6);
                auth = b64decode(auth);

                splitted = split(auth, ':');
                if (splitted.size() != 2)
                    return _404();

                const auto request_user = splitted.at(0);
                const auto request_passwd = splitted.at(1);

                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string();

                if (request_user == user and request_passwd == passwd)
                    out << "authenticated: true\n";
                else
                    out << "authenticated: false\n";
                out << "user: " << request_user << "\n";
                out << "password: " << request_passwd;
                
                return out.str();
            }

            string_t delay() {
                std::ostringstream out;

                const auto request_path = request.uri.path().value();
                const auto ind = request_path.find_last_of("/");
                if (ind == string_t::npos or ind == request_path.size())
                    return _404();

                const auto n =
                    std::strtoul(request_path.substr(ind + 1).c_str(), nullptr, 0);
                std::this_thread::sleep_for(seconds_t{n});

                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string();
                
                return out.str();
            }

            string_t params() {
                std::ostringstream out;

                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string();
                out << "query: " << request.uri.query();

                return out.str();
            }

            string_t cookies() {
                std::ostringstream out;

                headers.insert("Set-Cookie", "cookie1; Expires=Wed, 09 Jun 2021 10:18:14 GMT; HttpOnly");
                headers.insert("Set-Cookie", "cookie2");

                std::cout << __FUNCTION__ << ": headers: " << headers.to_string() << std::endl;

                out << "HTTP/1.1 200 OK\r\n";
                out << headers.to_string();

                return out.str();
            }
        
        public:
            headers_t headers {SERVER_DEFAULT_HEADERS};
            server_request_t request {};
        };

        class server_session_t
            : public std::enable_shared_from_this<server_session_t> {
        public:
            template <class StreamT>
            server_session_t(StreamT&& stream_)
                : stream(std::move(stream_))
            {
                
            }

            ~server_session_t() {
                
            }

            void start() {
                handshake();
            }

            void handshake() {
                auto self(shared_from_this());
                auto callback = [this, self](const ec_t& ec) {
                    if (ec) {
                        return;
                    }
                    read_method();
                };
                stream.async_handshake(callback);
            }

            void read_method() {
                auto self(shared_from_this());
                auto callback = [this, self](ec_t ec, std::size_t length) {
                    on_read_method(ec, length);
                };
                stream.async_read_until(request_buf,
                                        "\r\n",
                                        callback);
            }

            void on_read_method(ec_t ec, std::size_t ) {
                if (ec) {
                    return;
                }

                std::istream request_stream(&request_buf);
                string_t method;
                string_t url;
                request_stream >> method >> url >> std::ws;
                request.method = method_t{method};
                request.uri.domain("127.0.0.1"_domain);
                
                auto ind = url.find("?");
                if (ind != string_t::npos) {
                    request.uri.query(query_t{url.substr(ind + 1)});
                    request.uri.path(path_t{url.substr(0, ind)});
                }
                else {
                    request.uri.path(path_t{url});
                }
                
                request.prepare();

                if (not predefined_behaviour())
                    read_headers();
            }

            void read_headers() {
                auto self(shared_from_this());
            
                auto callback = [this, self](ec_t ec, std::size_t length) {
                    on_read_headers(ec, length);
                };
                stream.async_read_until(request_buf,
                                        "\r\n\r\n",
                                        callback);
            }

            void on_read_headers(ec_t ec, std::size_t) {
                if (ec) {
                    return;
                }

                request.headers = parse_headers(request_buf);
                response.request = request;

                write();
            }

            void write() {
                std::ostream response_stream(&response_buf);
            
                if (not predefined_behaviour(response_stream))
                    response_stream << response.make_http_response();
                response_stream.flush();

                auto self(shared_from_this());
                auto callback = [this, self](const ec_t& ec,
                                             const std::size_t length) {
                    on_write(ec, length);
                };
                stream.async_write(response_buf, callback);
            }

            void on_write(const ec_t& ec, const std::size_t ) {
                if (ec) {
                    return;
                }
            }

            bool predefined_behaviour(std::ostream& response_stream) {
                if (request.uri.path() == "/"_path) {
                    response_stream << response.make_http_response();
                    return true;
                }
                else if (request.uri.path() == "/get_wrong_status_data"_path) {
                    response_stream << response.get_wrong_status_data();
                    return true;
                }
                else if (request.uri.path() == "/get_wrong_status"_path) {
                    response_stream << "";
                    return true;
                }
                else if (request.uri.path() == "/get_wrong_headers"_path) {
                    response_stream << response.get_wrong_headers();
                    return true;
                }
                else if (request.uri.path() == "/get_wrong_content"_path) {
                    response_stream << response.get_wrong_content();
                    return true;
                }
                else if (request.uri.path() == "/get_wrong_chunk_data"_path) {
                    response_stream << response.get_wrong_chunk_data();
                    return true;
                }
                else if (request.uri.path() == "/get_response_with_data"_path) {
                    response_stream << response.get_response_with_data();
                    return true;
                }
                else if (request.uri.path() == "/get_content_length"_path) {
                    response_stream << response.get_content_length();
                    return true;
                }
                else if (request.uri.path() == "/get_big_content_length"_path) {
                    response_stream << response.get_big_content_length();
                    return true;
                }
                else if (request.uri.path() == "/get_big_chunks"_path) {
                    response_stream << response.get_big_chunks();
                    return true;
                }
                else if (request.uri.path() == "/get_big_until_eof"_path) {
                    response_stream << response.get_big_until_eof();
                    return true;
                }
                else if (request.uri.path() == "/ip"_path) {
                    response_stream << response.ip(stream);
                    return true;
                }
                else if (request.uri.path() == "/user-agent"_path) {
                    response_stream << response.user_agent();
                    return true;
                }
                else if (request.uri.path() == "/get"_path) {
                    response_stream << response.get();
                    return true;
                }
                else if (request.uri.path() == "/gzip"_path) {
                    response_stream << response.gzip();
                    return true;
                }
                else if (request.uri.path().value().find("/redirect") != string_t::npos) {
                    response_stream << response.redirect();
                    return true;
                }
                else if (request.uri.path().value().find("/basic_auth") != string_t::npos) {
                    response_stream << response.basic_auth();
                    return true;
                }
                else if (request.uri.path().value().find("/delay") != string_t::npos) {
                    response_stream << response.delay();
                    return true;
                }
                else if (request.uri.path() == "/params"_path) {
                    response_stream << response.params();
                    return true;
                }
                else if (request.uri.path() == "/cookies"_path) {
                    response_stream << response.cookies();
                    return true;
                }
                else {
                    response_stream << response._404();
                    return true;
                }
                return false;
            }

            bool predefined_behaviour() {
                if (request.uri.path() == "/read_status_error"_path) {
                    stream.close();
                    return true;
                }
                return false;
            }

        private:
            stream_t stream;
            streambuf_t request_buf {};
            streambuf_t response_buf {};
            server_request_t request {};
            server_response_t response {};
        };

    } /* anonymous namespace */

    server_t::server_t(const string_t& address,
                       const string_t& port,
                       bool is_ssl_)
        : io_service{},
          acceptor{io_service},
          is_ssl{is_ssl_}
    {
        resolver_t resolver{io_service};
        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});
        acceptor.open(endpoint.protocol());
        acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor.bind(endpoint);
        acceptor.listen();
        
        do_accept();
    }

    server_t::~server_t() {
        stop();
    }

    void server_t::run() {
        io_service.run();
    }

    void server_t::stop() {
        acceptor.close();
        io_service.stop();
    }

    void server_t::do_accept() {
        const auto stream = std::make_shared<stream_t>(io_service, is_ssl);
        
        const auto callback = [this, stream](ec_t ec) {
            if (not acceptor.is_open())
                return;
            
            if (not ec) {
                std::make_shared<server_session_t>(*stream)->start();
            }
            
            do_accept();
        };

        acceptor.async_accept(
            stream->socket<ssl_socket_t::lowest_layer_type>(), callback);
    }

} /* namespace crequests */
