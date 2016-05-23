#ifndef REQUEST_H
#define REQUEST_H

#include "macros.h"
#include "types.h"
#include "headers.h"
#include "uri.h"
#include "auth.h"
#include "cookies.h"

namespace crequests {


    declare_string(method);
    declare_number(timeout, size_t);
    declare_number(store_timeout, size_t);
    declare_bool(redirect);
    declare_number(redirect_count, size_t);
    declare_bool(gzip);
    declare_string(data);
    declare_bool(keep_alive);
    declare_bool(cache_redirects);

    
    const headers_t DEFAULT_HEADERS {
        {"Accept", "*/*"},
        {"Accept-Encoding", "gzip, deflate"},
        {"Connection", "close"},
        {"User-Agent", "Mozilla/5.0 (X11; Linux x86_64) "
                       "AppleWebKit/537.36 (KHTML, like Gecko) "
                       "Chrome/47.0.2526.106 Safari/537.36"}};


    class request_t {
    public:
        request_t();
        request_t(const request_t& request);
        request_t(request_t&& request);
        request_t& operator=(const request_t& request);
        ~request_t();

    public:
        void prepare();
        string_t make_request();
        bool is_ssl() const;

    public:
        void uri(const uri_t& uri);
        void url(const string_t& url);
        void url(const url_t& url);
        void protocol(const protocol_t& protocol);
        void domain(const domain_t& domain);
        void port(const port_t& port);
        void path(const path_t& path);
        void query(const query_t& query);
        void params(const params_t& params);

        void uri(uri_t&& uri);
        void url(string_t&& url);
        void url(url_t&& url);
        void protocol(protocol_t&& protocol);
        void domain(domain_t&& domain);
        void port(port_t&& port);
        void path(path_t&& path);
        void query(query_t&& query);
        void params(params_t&& params);

        void method(const method_t& method);
        void timeout(const timeout_t& timeout);
        void store_timeout(const store_timeout_t& store_timeout);
        void redirect(const redirect_t& redirect);
        void redirect_count(const redirect_count_t& redirect_count);
        void gzip(const gzip_t& gzip);
        void data(const data_t& data);
        void headers(const headers_t& headers);
        void final_callback(const final_callback_t& final_callback);
        void auth(const auth_t& auth);
        void keep_alive(const keep_alive_t& keep_alive);
        void cache_redirects(const cache_redirects_t& cache_redirects);
        void cookies(const cookies_t& cookies);
        
        void method(method_t&& method);
        void timeout(timeout_t&& timeout);
        void store_timeout(store_timeout_t&& store_timeout);
        void redirect(redirect_t&& redirect);
        void redirect_count(redirect_count_t&& redirect_count);
        void gzip(gzip_t&& gzip);
        void data(data_t&& data);
        void headers(headers_t&& headers);
        void final_callback(final_callback_t&& final_callback);
        void auth(auth_t&& auth);
        void keep_alive(keep_alive_t&& keep_alive);
        void cache_redirects(cache_redirects_t&& cache_redirects);
        void cookies(cookies_t&& cookies);

        const uri_t& uri() const;
        const method_t& method() const;
        const timeout_t& timeout() const;
        const store_timeout_t& store_timeout() const;
        const redirect_t& redirect() const;
        const redirect_count_t& redirect_count() const;
        const gzip_t& gzip() const;
        const data_t& data() const;
        const headers_t& headers() const;
        const final_callback_t& final_callback() const;
        const auth_t& auth() const;
        const keep_alive_t& keep_alive() const;
        const cache_redirects_t& cache_redirects() const;
        const cookies_t& cookies() const;
        
    private:
        uri_t m_uri {};
        method_t m_method { "GET" };
        timeout_t m_timeout { 120 };
        store_timeout_t m_store_timeout { 60 };
        redirect_t m_redirect { true };
        redirect_count_t m_redirect_count { 10 };
        gzip_t m_gzip { false };
        data_t m_data {};
        keep_alive_t m_keep_alive { true };
        headers_t m_headers { DEFAULT_HEADERS };
        final_callback_t m_final_callback {[](const response_t&){}};
        auth_t m_auth {};
        cache_redirects_t m_cache_redirects { true };
        cookies_t m_cookies;
    };


    std::ostream& operator<<(std::ostream& out, const request_t& request);


} /* namespace crequests */

#endif /* REQUEST_H */
