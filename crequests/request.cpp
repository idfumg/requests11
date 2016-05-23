#include "request.h"
#include "connection.h"
#include "utils.h"

#include <iostream>
#include <sstream>

namespace requests {


    request_t::request_t() {
        
    }

    request_t::request_t(const request_t& request)
        : m_uri {request.m_uri},
          m_method {request.m_method},
          m_timeout {request.m_timeout},
          m_store_timeout {request.m_store_timeout},
          m_redirect {request.m_redirect},
          m_redirect_count {request.m_redirect_count},
          m_gzip {request.m_gzip},
          m_data {request.m_data},
          m_keep_alive {request.m_keep_alive},
          m_headers {request.m_headers},
          m_final_callback {request.m_final_callback},
          m_auth {request.m_auth},
          m_cache_redirects {request.m_cache_redirects},
          m_cookies {request.m_cookies}
    {
        
    }

    request_t::request_t(request_t&& request)
        : m_uri {std::move(request.m_uri)},
          m_method {std::move(request.m_method)},
          m_timeout {std::move(request.m_timeout)},
          m_store_timeout {std::move(request.m_store_timeout)},
          m_redirect {std::move(request.m_redirect)},
          m_redirect_count {std::move(request.m_redirect_count)},
          m_gzip {std::move(request.m_gzip)},
          m_data {std::move(request.m_data)},
          m_keep_alive {std::move(request.m_keep_alive)},
          m_headers {std::move(request.m_headers)},
          m_final_callback {std::move(request.m_final_callback)},
          m_auth {std::move(request.m_auth)},
          m_cache_redirects {std::move(request.m_cache_redirects)},
          m_cookies {std::move(request.m_cookies)}
    {
        
    }

    request_t& request_t::operator=(const request_t& request) {
        if (this != &request) {
            m_uri = request.m_uri;
            m_method = request.m_method;
            m_timeout = request.m_timeout;
            m_store_timeout = request.m_store_timeout;
            m_redirect = request.m_redirect;
            m_redirect_count = request.m_redirect_count;
            m_gzip = request.m_gzip;
            m_data = request.m_data;
            m_keep_alive = request.m_keep_alive;
            m_headers = request.m_headers;
            m_final_callback = request.m_final_callback;
            m_auth = request.m_auth;
            m_cache_redirects = request.m_cache_redirects;
            m_cookies = request.m_cookies;
        }

        return *this;
    }

    request_t::~request_t() {
        
    }


    /****************************************************************************
     * Set. Constant reference. Uri.
     ***************************************************************************/


    void request_t::uri(const uri_t& uri) {
        m_uri = uri;
    }
    
    void request_t::url(const string_t& url) {
        m_uri.url(url_t{url});
    }

    void request_t::url(const url_t& url) {
        m_uri.url(url);
    }

    void request_t::protocol(const protocol_t& protocol) {
        m_uri.protocol(protocol);
    }

    void request_t::domain(const domain_t& domain) {
        m_uri.domain(domain);
    }

    void request_t::port(const port_t& port) {
        m_uri.port(port);
    }

    void request_t::path(const path_t& path) {
        m_uri.path(path);
    }

    void request_t::query(const query_t& query) {
        m_uri.query(query);
    }

    void request_t::params(const params_t& params) {
        m_uri.params(params);
    }


    /****************************************************************************
     * Set. Rvalue reference. Uri.
     ***************************************************************************/


    void request_t::uri(uri_t&& uri) {
        m_uri = std::move(uri);
    }
    
    void request_t::url(string_t&& url) {
        m_uri.url(url_t{std::move(url)});
    }

    void request_t::url(url_t&& url) {
        m_uri.url(std::move(url));
    }

    void request_t::protocol(protocol_t&& protocol) {
        m_uri.protocol(std::move(protocol));
    }

    void request_t::domain(domain_t&& domain) {
        m_uri.domain(std::move(domain));
    }

    void request_t::port(port_t&& port) {
        m_uri.port(std::move(port));
    }

    void request_t::path(path_t&& path) {
        m_uri.path(std::move(path));
    }

    void request_t::query(query_t&& query) {
        m_uri.query(std::move(query));
    }

    void request_t::params(params_t&& params) {
        m_uri.params(std::move(params));
    }


    /****************************************************************************
     * Set. Constant reference.
     ***************************************************************************/


    void request_t::method(const method_t& method) {
        m_method = method;
    }

    void request_t::timeout(const timeout_t& timeout) {
        m_timeout = timeout;
    }

    void request_t::store_timeout(const store_timeout_t& store_timeout) {
        m_store_timeout = store_timeout;
    }

    void request_t::redirect(const redirect_t& redirect) {
        m_redirect = redirect;
    }

    void request_t::redirect_count(const redirect_count_t& redirect_count) {
        m_redirect_count = redirect_count;
    }

    void request_t::gzip(const gzip_t& gzip) {
        m_gzip = gzip;
    }

    void request_t::data(const data_t& data) {
        m_data = data;
    }

    void request_t::headers(const headers_t& headers) {
        m_headers = headers;
    }

    void request_t::final_callback(const final_callback_t& final_callback) {
        m_final_callback = final_callback;
    }

    void request_t::auth(const auth_t& auth) {
        m_auth = auth;
    }

    void request_t::keep_alive(const keep_alive_t& keep_alive) {
        m_keep_alive = keep_alive;
    }

    void request_t::cache_redirects(const cache_redirects_t& cache_redirects) {
        m_cache_redirects = cache_redirects;
    }

    void request_t::cookies(const cookies_t& cookies) {
        m_cookies = cookies;
    }


    /****************************************************************************
     * Set. Rvalue reference.
     ***************************************************************************/


    void request_t::method(method_t&& method) {
        m_method = std::move(method);
    }

    void request_t::timeout(timeout_t&& timeout) {
        m_timeout = std::move(timeout);
    }

    void request_t::store_timeout(store_timeout_t&& store_timeout) {
        m_store_timeout = std::move(store_timeout);
    }

    void request_t::redirect(redirect_t&& redirect) {
        m_redirect = std::move(redirect);
    }

    void request_t::redirect_count(redirect_count_t&& redirect_count) {
        m_redirect_count = std::move(redirect_count);
    }

    void request_t::gzip(gzip_t&& gzip) {
        m_gzip = std::move(gzip);
    }

    void request_t::data(data_t&& data) {
        m_data = std::move(data);
    }

    void request_t::headers(headers_t&& headers) {
        m_headers = std::move(headers);
    }

    void request_t::final_callback(final_callback_t&& final_callback) {
        m_final_callback = std::move(final_callback);
    }

    void request_t::auth(auth_t&& auth) {
        m_auth = std::move(auth);
    }

    void request_t::keep_alive(keep_alive_t&& keep_alive) {
        m_keep_alive = std::move(keep_alive);
    }

    void request_t::cache_redirects(cache_redirects_t&& cache_redirects) {
        m_cache_redirects = std::move(cache_redirects);
    }

    void request_t::cookies(cookies_t&& cookies) {
        m_cookies = std::move(cookies);
    }


    /****************************************************************************
     * Get. Constant reference.
     ***************************************************************************/


    const uri_t& request_t::uri() const {
        return m_uri;
    }

    const method_t& request_t::method() const {
        return m_method;
    }

    const timeout_t& request_t::timeout() const {
        return m_timeout;
    }

    const store_timeout_t& request_t::store_timeout() const {
        return m_store_timeout;
    }

    const redirect_t& request_t::redirect() const {
        return m_redirect;
    }

    const redirect_count_t& request_t::redirect_count() const {
        return m_redirect_count;
    }

    const gzip_t& request_t::gzip() const {
        return m_gzip;
    }

    const data_t& request_t::data() const {
        return m_data;
    }

    const headers_t& request_t::headers() const {
        return m_headers;
    }

    const final_callback_t& request_t::final_callback() const {
        return m_final_callback;
    }

    const auth_t& request_t::auth() const {
        return m_auth;
    }

    const keep_alive_t& request_t::keep_alive() const {
        return m_keep_alive;
    }

    const cache_redirects_t& request_t::cache_redirects() const {
        return m_cache_redirects;
    }

    const cookies_t& request_t::cookies() const {
        return m_cookies;
    }


    /****************************************************************************
     * Other functions.
     ***************************************************************************/


    string_t request_t::make_request() {
        assert(not m_method.empty());
        assert(not m_uri.path().empty());
        assert(not m_uri.domain().empty());
        
        std::ostringstream out;
        
        out << m_method << " " << m_uri.path();
        
        if (not m_uri.query().empty())
            out << "?" + m_uri.query().value();

        auto cookies =
            m_cookies.get(m_uri.domain().value(), m_uri.path().value());
        auto headers_ = m_headers;
        
        if (not cookies.empty())
            headers_.insert("Cookies", cookies.to_string());
        
        out << " " << "HTTP/1.1\r\n";
        out << headers_.to_string();
        
        if (not m_data.empty()) {
            if (m_gzip)
                out << compress(m_data.value());
            else
                out << m_data;
        }

        return out.str();
    }

    void request_t::prepare()  {
        m_uri.prepare();
        assert(not m_uri.domain().empty() or not m_uri.url().empty());
        if (m_gzip)
            m_headers.insert("Content-Encoding", "gzip");
        if (not m_auth.first.empty() and not m_auth.second.empty())
            m_headers.insert("Authorization",
                             "Basic " + b64encode(m_auth.to_string()));
        if (m_keep_alive)
            m_headers.insert("Connection", "keep-alive");
        if (not m_data.empty())
            m_headers.insert("Content-Length",
                              std::to_string(m_data.value().size()));
        m_headers.insert("Host", m_uri.domain().value());
    }

    bool request_t::is_ssl() const {
        return uri().protocol().value() == "https";
    }

    std::ostream& operator<<(std::ostream& out, const request_t& request) {
        out << "Request: "
            << request.uri()
            << "timeout: " << request.timeout() << "\n\n"
            << request.headers() << "\n\n"
            << request.data() << "\n";

        return out;
    }


} /* namespace requests */
