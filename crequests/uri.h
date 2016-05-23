#ifndef URI_H
#define URI_H

#include "macros.h"
#include "params.h"

namespace crequests {


    declare_string(url);
    declare_string(protocol);
    declare_string(domain);
    declare_string(port);
    declare_string(path);
    declare_string(fragment);
    declare_string(query);


    const string_t DEFAULT_PROTOCOL = "http";
    const string_t DEFAULT_SSL_PROTOCOL = "https";
    const string_t DEFAULT_PORT = "80";
    const string_t DEFAULT_SSL_PORT = "443";
    const string_t DEFAULT_PATH = "/";


    class uri_t {
    public:
        uri_t();
        uri_t(const uri_t& uri);
        uri_t(uri_t&& uri);
        uri_t& operator=(const uri_t& uri);
        ~uri_t();

    public:
        static uri_t from_string(const string_t& str);
        void prepare();
        url_t make_url() const;
        void update(const uri_t& uri);
        void update(uri_t&& uri);

    public:
        void url(const url_t& url);
        void protocol(const protocol_t& protocol);
        void domain(const domain_t& domain);
        void port(const port_t& port);
        void path(const path_t& path);
        void fragment(const fragment_t& fragment);
        void query(const query_t& query);
        void params(const params_t& params);

        void url(url_t&& url);
        void protocol(protocol_t&& protocol);
        void domain(domain_t&& domain);
        void port(port_t&& port);
        void path(path_t&& path);
        void fragment(fragment_t&& fragment);
        void query(query_t&& query);
        void params(params_t&& params);

        const url_t& url() const;
        const protocol_t& protocol() const;
        const domain_t& domain() const;
        const port_t& port() const;
        const path_t& path() const;
        const fragment_t& fragment() const;
        const query_t& query() const;
        const params_t& params() const;

    private:
        void set_defaults();

    private:
        url_t m_url;
        protocol_t m_protocol;
        domain_t m_domain;
        port_t m_port;
        path_t m_path;
        fragment_t m_fragment;
        query_t m_query;
        params_t m_params;
    };


    std::ostream& operator<<(std::ostream& out, const uri_t& uri);
    uri_t operator "" _uri(const char* val, size_t);


} /* namespace crequests */

#endif /* URI_H */
