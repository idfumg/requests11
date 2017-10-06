#ifndef SESSION_H
#define SESSION_H

#include "request.h"
#include "utils.h"
#include "auth.h"
#include "response.h"
#include "asyncresponse.h"

namespace crequests {

    class session_t {
    public:
        session_t(service_t& service);
        session_t(const session_t& session);
        session_t(session_t&& session);
        session_t& operator=(const session_t& session);
        session_t& operator=(session_t&& session);
        ~session_t();

    public:
        asyncresponse_t AsyncGet();
        asyncresponse_t AsyncPost();
        asyncresponse_t AsyncPut();
        asyncresponse_t AsyncPatch();
        asyncresponse_t AsyncDelete();
        asyncresponse_t AsyncHead();
        asyncresponse_t AsyncSend();

        response_t Get();
        response_t Post();
        response_t Put();
        response_t Patch();
        response_t Delete();
        response_t Head();
        response_t Send();

        void set_option(const string_t& url);
        void set_option(const url_t& url);
        void set_option(const protocol_t& protocol);
        void set_option(const domain_t& domain);
        void set_option(const port_t& port);
        void set_option(const path_t& path);
        void set_option(const query_t& query);
        void set_option(const params_t& params);
        void set_option(const method_t& method);
        void set_option(const timeout_t& timeout);
        void set_option(const store_timeout_t& store_timeout);
        void set_option(const redirect_t& redirect);
        void set_option(const redirect_count_t& redirect_count);
        void set_option(const gzip_t& gzip);
        void set_option(const headers_t& headers);
        void set_option(const final_callback_t& final_callback);
        void set_option(const data_t& data);
        void set_option(const auth_t& auth);
        void set_option(const keep_alive_t& keep_alive);
        void set_option(const cache_redirects_t& cache_redirects);
        void set_option(const cookies_t& cookies);
        void set_option(const throw_on_error_t& throw_on_error);
        void set_option(const body_callback_t& body_callback);
        void set_option(const ssl_auth_t& ssl_auth);
        void set_option(const ssl_certs_t& ssl_certs);
        void set_option(const always_verify_peer_t& always_verify_peer);
        void set_option(const verify_path_t& verify_path);
        void set_option(const verify_filename_t& verify_filename);
        void set_option(const certificate_file_t& certificate_file);
        void set_option(const private_key_file_t& private_key_file);

        void set_option(string_t&& url);
        void set_option(url_t&& url);
        void set_option(protocol_t&& protocol);
        void set_option(domain_t&& domain);
        void set_option(port_t&& port);
        void set_option(path_t&& path);
        void set_option(query_t&& query);
        void set_option(params_t&& params);
        void set_option(method_t&& method);
        void set_option(timeout_t&& timeout);
        void set_option(store_timeout_t&& store_timeout);
        void set_option(redirect_t&& redirect);
        void set_option(redirect_count_t&& redirect_count);
        void set_option(gzip_t&& gzip);
        void set_option(headers_t&& headers);
        void set_option(final_callback_t&& final_callback);
        void set_option(data_t&& data);
        void set_option(auth_t&& auth);
        void set_option(keep_alive_t&& keep_alive);
        void set_option(cache_redirects_t&& cache_redirects);
        void set_option(cookies_t&& cookies);
        void set_option(throw_on_error_t&& throw_on_error);
        void set_option(body_callback_t&& body_callback);
        void set_option(ssl_auth_t&& ssl_auth);
        void set_option(ssl_certs_t&& ssl_certs);
        void set_option(always_verify_peer_t&& always_verify_peer);
        void set_option(verify_path_t&& verify_path);
        void set_option(verify_filename_t&& verify_filename);
        void set_option(certificate_file_t&& certificate_file);
        void set_option(private_key_file_t&& private_key_file);

        bool is_expired() const;

    private:
        friend class session_impl_t;
        shared_ptr_t<class session_impl_t> pimpl;
    };

    
} /* namespace crequests */

#endif /* SESSION_H */
