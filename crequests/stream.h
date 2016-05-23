#ifndef SOCKET_H
#define SOCKET_H

#include "boost_asio.h"

namespace requests {
    
    template <class ServiceT>
    static inline ssl_socket_ptr_t create_ssl_socket(ServiceT&& service, bool is_server) {
        if (not is_server) {
            boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
            ctx.set_verify_mode(boost::asio::ssl::verify_none);
            ctx.set_default_verify_paths();
            return std::make_shared<ssl_socket_t>(service, ctx);
        }
        else {
            boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
            ctx.set_verify_mode(boost::asio::ssl::verify_none);
            ctx.set_default_verify_paths();
            ctx.use_certificate_chain_file("cert/server.crt");
            ctx.use_private_key_file("cert/server.key", boost::asio::ssl::context::pem);
            return std::make_shared<ssl_socket_t>(service, ctx);
        }
    }

    template <class ServiceT>
    static inline tcp_socket_ptr_t create_tcp_socket(ServiceT&& service) {
        return std::make_shared<tcp_socket_t>(service);
    }

    class stream_t {
    public:
        template <class ServiceT>
        stream_t(ServiceT&& service,
                 bool is_ssl = false,
                 bool is_server = false) {
            if (is_ssl)
                ssl_socket = create_ssl_socket(service, is_server);
            else
                tcp_socket = create_tcp_socket(service);
            if (is_server)
                type = boost::asio::ssl::stream_base::server;
            else
                type = boost::asio::ssl::stream_base::client;
        }

        stream_t(stream_t&& stream) {
            ssl_socket = stream.ssl_socket;
            tcp_socket = stream.tcp_socket;
            type = stream.type;
            stream.ssl_socket = nullptr;
            stream.tcp_socket = nullptr;
        }

        stream_t(const stream_t& stream) = default;
        stream_t& operator = (const stream_t& stream) = default;

        ~stream_t() {
            close();
        }

        void cancel_() {
            if (ssl_socket and ssl_socket->lowest_layer().is_open()) {
                ssl_socket->lowest_layer().cancel();
                ssl_socket->shutdown();
            }
            else if (tcp_socket and tcp_socket->is_open()) {
                tcp_socket->cancel();
                tcp_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            }
        }

        void cancel() {
            try {
                cancel_();
            } catch (...) {
                
            }
        }

        void close_() {
            if (ssl_socket and ssl_socket->lowest_layer().is_open()) {
                ssl_socket->lowest_layer().close();
                ssl_socket.reset();
                ssl_socket = nullptr;
            }
            else if (tcp_socket and tcp_socket->is_open()) {
                tcp_socket->close();
                tcp_socket.reset();
                tcp_socket = nullptr;
            }
        }

        void close() {
            try {
                close_();
            } catch (...) {
                
            }
        }

    public:
        template <class SocketT>
        SocketT& socket() {
            if (tcp_socket)
                return *tcp_socket;
            else if (ssl_socket)
                return ssl_socket->lowest_layer();
            throw std::runtime_error("no live socket in stream!");
        }
        
        template <class... Args>
        void async_connect(Args&& ...args) {
            if (tcp_socket)
                boost::asio::async_connect(*tcp_socket,
                                           std::forward<Args>(args)...);
            else if (ssl_socket)
                boost::asio::async_connect(ssl_socket->lowest_layer(),
                                           std::forward<Args>(args)...);
        }

        template <class CallbackT, class... Args>
        void async_handshake(CallbackT&& callback, Args&& ...args) {
            if (ssl_socket and ssl_socket->lowest_layer().is_open())
                ssl_socket->async_handshake(type,
                                            std::forward<CallbackT>(callback),
                                            std::forward<Args>(args)...);
            else if (tcp_socket and tcp_socket->is_open())
                callback(ec_t());
        }

        template <class... Args>
        void async_write(Args&& ...args) {
            if (tcp_socket and tcp_socket->is_open())
                boost::asio::async_write(*tcp_socket,
                                         std::forward<Args>(args)...);
            else if (ssl_socket and ssl_socket->lowest_layer().is_open())
                boost::asio::async_write(*ssl_socket,
                                         std::forward<Args>(args)...);
        }

        template <class... Args>
        void async_read_until(Args&& ...args) {
            if (tcp_socket and tcp_socket->is_open())
                boost::asio::async_read_until(*tcp_socket,
                                              std::forward<Args>(args)...);
            else if (ssl_socket and ssl_socket->lowest_layer().is_open())
                boost::asio::async_read_until(*ssl_socket,
                                              std::forward<Args>(args)...);
        }

        template <class... Args>
        void async_read(Args&& ...args) {
            if (tcp_socket and tcp_socket->is_open())
                boost::asio::async_read(*tcp_socket,
                                        std::forward<Args>(args)...);
            else if (ssl_socket and ssl_socket->lowest_layer().is_open())
                boost::asio::async_read(*ssl_socket,
                                        std::forward<Args>(args)...);
        }

        template <class OptionT>
        void set_option(OptionT&& option) {
            if (tcp_socket and tcp_socket->is_open())
                tcp_socket->set_option(option);
            else if (ssl_socket and ssl_socket->lowest_layer().is_open())
                ssl_socket->lowest_layer().set_option(option);
        }

        template <class OptionT>
        bool get_option(OptionT&& option) {
            if (tcp_socket and tcp_socket->is_open())
                tcp_socket->get_option(option);
            else if (ssl_socket and ssl_socket->lowest_layer().is_open())
                ssl_socket->lowest_layer().get_option(option);
            
            return option.value();
        }

        bool is_open() {
            if (tcp_socket and tcp_socket->is_open())
                return true;
            else if (ssl_socket and ssl_socket->lowest_layer().is_open())
                return true;
            return false;
        }

    private:
        tcp_socket_ptr_t tcp_socket { nullptr };
        ssl_socket_ptr_t ssl_socket { nullptr };
        boost::asio::ssl::stream_base::handshake_type type;
    };

} /* namespace requests */

#endif /* SOCKET_H */
