#ifndef SOCKET_H
#define SOCKET_H

#include "boost_asio.h"
#include "request.h"

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <iostream>

namespace crequests {

    static inline shared_ptr_t<BIO> NewStrBIO(const string_t& str) {
        BIO* bio = BIO_new_mem_buf(const_cast<char*>(str.data()), static_cast<int>(str.size()));
        if (bio == NULL)
            throw std::runtime_error("BIO_new_mem_buf failed");
        return shared_ptr_t<BIO>(bio, BIO_vfree);
    }

    static inline shared_ptr_t<X509> NewX509(const string_t& cert) {
        auto cert_bio = NewStrBIO(cert);
        X509* x509_cert = PEM_read_bio_X509(cert_bio.get(), NULL, NULL, NULL);
        if (x509_cert == NULL)
            throw std::runtime_error("create new X509 cert failed");
        return shared_ptr_t<X509>(x509_cert, X509_free);
    }

    static inline shared_ptr_t<EVP_PKEY> NewEVP_PKEY(const string_t& pkey) {
        auto pkey_bio = NewStrBIO(pkey);
        EVP_PKEY* evp_pkey = ::PEM_read_bio_PrivateKey(pkey_bio.get(), NULL, NULL, NULL);
        if (evp_pkey == NULL)
            throw std::runtime_error("create new private key failed");
        return shared_ptr_t<EVP_PKEY>(evp_pkey, EVP_PKEY_free);
    }

    static inline void UseCertAndKey(SSL_CTX* ctx, X509* cert, EVP_PKEY* pkey) {
        if (!X509_check_private_key(cert, pkey))
            throw std::runtime_error("private key mismatch cert certificate");

        if (SSL_CTX_use_certificate(ctx, cert) != 1)
            throw std::runtime_error("using certificate failed");

        if (SSL_CTX_use_PrivateKey(ctx, pkey) != 1)
            throw std::runtime_error("using private key failed");
    }

    static inline void UseCACerts(SSL_CTX* ctx,
                                  const vector_t<shared_ptr_t<X509> >& certs) {
        X509_STORE* x509_store = X509_STORE_new();
        if (x509_store == NULL)
            throw std::runtime_error("creating new X509 cert failed");

        for (auto&& cert:  certs)
            if (!X509_STORE_add_cert(x509_store, cert.get()))
                throw std::runtime_error("add cert to X509 store failed");

        SSL_CTX_set_cert_store(ctx, x509_store);
    }

    template <class ServiceT>
    static inline ssl_socket_ptr_t create_ssl_socket_client(
        ServiceT&& service,
        const shared_ptr_t<X509>& cert, const shared_ptr_t<EVP_PKEY>& key,
        const vector_t<shared_ptr_t<X509> >& certs,
        const always_verify_peer_t& always_verify_peer,
        const verify_path_t& verify_path,
        const verify_filename_t& verify_filename,
        const certificate_file_t& certificate_file,
        const private_key_file_t& private_key_file,
        const domain_t& domain)
    {
        boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23_client);
        ctx.set_verify_mode(boost::asio::ssl::verify_none);
        ctx.set_default_verify_paths();
        ctx.set_options(boost::asio::ssl::context::default_workarounds);

        if (cert and key)
            UseCertAndKey(ctx.impl(), cert.get(), key.get());

        if (not certs.empty())
            UseCACerts(ctx.impl(), certs);

        if (not verify_path.empty())
            ctx.add_verify_path(verify_path.value());

        if (not verify_filename.empty())
            ctx.load_verify_file(verify_filename.value());

        if (not certificate_file.empty())
            ctx.use_certificate_file(certificate_file.value(),
                                     boost::asio::ssl::context::pem);

        if (not private_key_file.empty())
            ctx.use_private_key_file(private_key_file.value(),
                                     boost::asio::ssl::context::pem);

        if ((cert and key) or
            not certs.empty() or
            always_verify_peer or
            not verify_path.empty() or
            not verify_filename.empty())
            ctx.set_verify_mode(boost::asio::ssl::verify_peer);

        auto socket = std::make_shared<ssl_socket_t>(service, ctx);

        if (not domain.empty() and always_verify_peer)
            socket->set_verify_callback(
                boost::asio::ssl::rfc2818_verification(domain.value()));

        return socket;
    }

    template <class ServiceT>
    static inline ssl_socket_ptr_t create_ssl_socket_server(ServiceT&& service)
    {
        boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23_server);
        ctx.set_verify_mode(boost::asio::ssl::verify_none);
        ctx.set_default_verify_paths();
        ctx.set_options(boost::asio::ssl::context::default_workarounds);

        ctx.use_certificate_chain_file("cert/server.crt");
        ctx.use_private_key_file("cert/server.key", boost::asio::ssl::context::pem);

        return std::make_shared<ssl_socket_t>(service, ctx);
    }

    template <class ServiceT>
    static inline tcp_socket_ptr_t create_tcp_socket(ServiceT&& service) {
        return std::make_shared<tcp_socket_t>(service);
    }

    class stream_t {
    public:
        template <class ServiceT>
        stream_t(ServiceT&& service, const request_t& request) {
            const ssl_auth_t& ssl_auth = request.ssl_auth();
            const ssl_certs_t& ssl_certs = request.ssl_certs();
            
            if (not ssl_auth.first.empty() and not ssl_auth.second.empty()) {
                cert = NewX509(ssl_auth.first.value());
                key = NewEVP_PKEY(ssl_auth.second.value());
            }

            if (not ssl_certs.empty())
                for (const auto& cert_ : ssl_certs)
                    certs.push_back(NewX509(cert_.value()));

            if (request.is_ssl()) {
                ssl_socket = create_ssl_socket_client(service,
                                                      cert, key,
                                                      certs,
                                                      request.always_verify_peer(),
                                                      request.verify_path(),
                                                      request.verify_filename(),
                                                      request.certificate_file(),
                                                      request.private_key_file(),
                                                      request.uri().domain());
            } else {
                tcp_socket = create_tcp_socket(service);
            }

            type = boost::asio::ssl::stream_base::client;
        }

        template <class ServiceT>
        stream_t(ServiceT&& service, bool is_ssl) {
            if (is_ssl)
                ssl_socket = create_ssl_socket_server(service);
            else
                tcp_socket = create_tcp_socket(service);
            type = boost::asio::ssl::stream_base::server;
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
        boost::asio::ssl::stream_base::handshake_type type{};

        shared_ptr_t<X509> cert {};
        shared_ptr_t<EVP_PKEY> key {};
        vector_t<shared_ptr_t<X509> > certs {};
    };

} /* namespace crequests */

#endif /* SOCKET_H */
