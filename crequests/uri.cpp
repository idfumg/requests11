#include "uri.h"
#include "types.h"
#include "utils.h"

#include <map>
#include <sstream>
#include <iostream>

namespace crequests {


    class uri_parser_t {
    private:
        enum state_t {
            UPROTOCOL = 0,
            UDOMAIN = 1,
            UPORT = 2,
            UPATH = 3,
            UFRAGMENT = 4,
            UQUERY = 5
        };
        
    public:
        uri_parser_t(const string_t& str) {
            uri.url(url_t{str});
            set_state(state_t::UPROTOCOL);
        }

        template <class IteratorT>
        inline void parse_protocol(IteratorT& first,
                                   IteratorT& cur,
                                   IteratorT& last) {
            if (cur != last and *cur == ':') {
                uri.protocol(protocol_t{string_t{first, cur}});
                
                if (cur + 1 != last and *(cur + 1) == '/' and
                    cur + 2 != last and *(cur + 2) == '/')
                    cur = cur + 2;
                
                first = cur + 1;
            }
        }

        template <class IteratorT>
        inline void parse_domain(IteratorT& first, IteratorT& cur) {
            uri.domain(domain_t{string_t{first, cur}});
            first = cur;
        }

        template <class IteratorT>
        inline void parse_port(IteratorT& first, IteratorT& cur) {
            uri.port(port_t{string_t{first + 1, cur}});
            first = cur;
        }

        template <class IteratorT>
        inline void parse_path(IteratorT& first, IteratorT& cur) {
            uri.path(path_t{string_t{first, cur}});
            first = cur;
        }

        template <class IteratorT>
        inline void parse_fragment(IteratorT& first, IteratorT& cur) {
            uri.fragment(fragment_t{string_t{first + 1, cur}});
            first = cur;
        }

        template <class IteratorT>
        inline void parse_query(IteratorT& first, IteratorT& cur) {
            uri.query(query_t{string_t{first + 1, cur}});
            first = cur;
        }

        inline void set_state(const state_t state_) {
            state = state_;
        }
        
        uri_t parse() {
            auto first = begin(uri.url().value());
            auto cur = begin(uri.url().value());
            auto last = end(uri.url().value());

            while (cur != last) {
                switch (*cur) {
                case ':':
                    if (state == state_t::UPROTOCOL)  {
                        parse_protocol(first, cur, last);
                        set_state(state_t::UDOMAIN);
                    }
                    else if (state == state_t::UDOMAIN) {
                        parse_domain(first, cur);
                        set_state(state_t::UPORT);
                    }
                    break;
                    
                case '.':
                    set_state(state_t::UDOMAIN);
                    break;

                case '/':
                    if (state == state_t::UDOMAIN)
                        parse_domain(first, cur);
                    else if (state == state_t::UPORT)
                        parse_port(first, cur);
                    else if (state == state_t::UQUERY)
                        break;
                    set_state(state_t::UPATH);
                    break;

                case '#':
                    if (state == state_t::UPATH)
                        parse_path(first, cur);
                    set_state(state_t::UFRAGMENT);
                    break;

                case '?':
                    if (state == state_t::UFRAGMENT)
                        parse_fragment(first, cur);
                    else if (state == state_t::UPATH)
                        parse_path(first, cur);
                    else if (state == state_t::UDOMAIN)
                        parse_domain(first, cur);
                    else if (state == state_t::UPORT)
                        parse_port(first, cur);
                    set_state(state_t::UQUERY);
                    break;
                };
                cur++;
            }

            switch (state) {
            case UPROTOCOL:
                parse_protocol(first, cur, last);
                break;
                
            case UDOMAIN:
                parse_domain(first, cur);
                break;
                
            case UPORT:
                parse_port(first, cur);
                break;

            case UPATH:
                parse_path(first, cur);
                break;
            
            case UFRAGMENT:
                parse_fragment(first, cur);
                break;

            case UQUERY:
                parse_query(first, cur);
                break;
            };

            assert(first == last);
            assert(cur == last);

            return uri;
        }

    private:
        uri_t uri;
        state_t state;
    };


    uri_t::uri_t() {
    }
    
    uri_t::uri_t(const uri_t& uri)
        : m_url {uri.m_url},
          m_protocol {uri.m_protocol},
          m_domain {uri.m_domain},
          m_port {uri.m_port},
          m_path {uri.m_path},
          m_fragment {uri.m_fragment},
          m_query {uri.m_query},
          m_params {uri.m_params}
    {
        
    }
    
    uri_t::uri_t(uri_t&& uri)
        : m_url {std::move(uri.m_url)},
          m_protocol {std::move(uri.m_protocol)},
          m_domain {std::move(uri.m_domain)},
          m_port {std::move(uri.m_port)},
          m_path {std::move(uri.m_path)},
          m_fragment {std::move(uri.m_fragment)},
          m_query {std::move(uri.m_query)},
          m_params {std::move(uri.m_params)}
    {
        
    }

    uri_t& uri_t::operator=(const uri_t& uri) {
        if (this != &uri) {
            m_url = uri.m_url;
            m_protocol = uri.m_protocol;
            m_domain = uri.m_domain;
            m_port = uri.m_port;
            m_path = uri.m_path;
            m_fragment = uri.m_fragment;
            m_query = uri.m_query;
            m_params = uri.m_params;
        }

        return *this;
    }

    uri_t::~uri_t() {
        
    }


    /****************************************************************************
     * Set. Constant reference.
     ***************************************************************************/


    void uri_t::url(const url_t& url) {
        m_url = url;
    }

    void uri_t::protocol(const protocol_t& protocol) {
        m_protocol = protocol;
    }

    void uri_t::domain(const domain_t& domain) {
        m_domain = domain;
    }

    void uri_t::port(const port_t& port) {
        m_port = port;
    }

    void uri_t::path(const path_t& path) {
        m_path = path;
    }

    void uri_t::fragment(const fragment_t& fragment) {
        m_fragment = fragment;
    }

    void uri_t::query(const query_t& query) {
        m_query = query;
    }

    void uri_t::params(const params_t& params) {
        m_params = params;
    }


    /****************************************************************************
     * Set. Rvalue reference.
     ***************************************************************************/


    void uri_t::url(url_t&& url) {
        m_url = std::move(url);
    }

    void uri_t::protocol(protocol_t&& protocol) {
        m_protocol = std::move(protocol);
    }

    void uri_t::domain(domain_t&& domain) {
        m_domain = std::move(domain);
    }

    void uri_t::port(port_t&& port) {
        m_port = std::move(port);
    }

    void uri_t::path(path_t&& path) {
        m_path = std::move(path);
    }

    void uri_t::fragment(fragment_t&& fragment) {
        m_fragment = std::move(fragment);
    }

    void uri_t::query(query_t&& query) {
        m_query = std::move(query);
    }

    void uri_t::params(params_t&& params) {
        m_params = std::move(params);
    }


    /****************************************************************************
     * Get. Constant reference.
     ***************************************************************************/


    const url_t& uri_t::url() const {
        return m_url;
    }

    const protocol_t& uri_t::protocol() const {
        return m_protocol;
    }

    const domain_t& uri_t::domain() const {
        return m_domain;
    }

    const port_t& uri_t::port() const {
        return m_port;
    }

    const path_t& uri_t::path() const {
        return m_path;
    }

    const fragment_t& uri_t::fragment() const {
        return m_fragment;
    }

    const query_t& uri_t::query() const {
        return m_query;
    }

    const params_t& uri_t::params() const {
        return m_params;
    }


    /****************************************************************************
     * Other functions.
     ***************************************************************************/


    uri_t uri_t::from_string(const string_t& str) {
        uri_parser_t parser(str);
        return parser.parse();
    }

    void uri_t::update(const uri_t& uri) {
        if (not uri.url().empty())
            m_url = uri.url();
        if (not uri.protocol().empty())
            m_protocol = uri.protocol();
        if (not uri.domain().empty())
            m_domain = uri.domain();
        if (not uri.port().empty())
            m_port = uri.port();
        if (not uri.path().empty())
            m_path = uri.path();
        if (not uri.fragment().empty())
            m_fragment = uri.fragment();

        if (not m_query.empty())
            if (is_url_encoded(m_query.value()))
                m_query = query_t{urldecode(m_query.value())};
        
        if (not uri.query().empty()) {
            query_t query = uri.query();
            if (is_url_encoded(query.value()))
                query = query_t{urldecode(query.value())};
            m_query.value().append("&").append(query.value());
        }
        
        m_params.update(uri.params());
    }

    void uri_t::update(uri_t&& uri) {
        if (not uri.url().empty())
            m_url = std::move(uri.url());
        if (not uri.protocol().empty())
            m_protocol = std::move(uri.protocol());
        if (not uri.domain().empty())
            m_domain = std::move(uri.domain());
        if (not uri.port().empty())
            m_port = std::move(uri.port());
        if (not uri.path().empty())
            m_path = std::move(uri.path());
        if (not uri.fragment().empty())
            m_fragment = std::move(uri.fragment());

        if (not uri.query().empty()) {
            query_t query = uri.query();
            if (is_url_encoded(query.value()))
                query = query_t{urldecode(query.value())};
            m_query.value().append("&").append(query.value());
        }
        
        m_params.update(uri.params());
    }

    void uri_t::prepare() {
        if (not m_url.empty())
            update(uri_t::from_string(m_url.value()));

        auto new_params = params_t::from_string(m_query.value());
        new_params.update(m_params);
        m_query = query_t(new_params.to_string());
        
        set_defaults();
        m_url = make_url();
    }

    void uri_t::set_defaults() {
        static const std::pair<string_t, string_t> tab[] = {
            {DEFAULT_PROTOCOL, DEFAULT_PORT},
            {DEFAULT_SSL_PROTOCOL, DEFAULT_SSL_PORT}
        };

        if (m_protocol.empty())
            for (auto&& pair : tab)
                if (m_port.value() == pair.second)
                    m_protocol.value() = pair.first;

        if (m_port.empty())
            for (auto&& pair : tab)
                if (m_protocol.value() == pair.first)
                    m_port.value() = pair.second;

        for (auto&& pair : tab)
            if (m_protocol.value() == pair.first and m_port.empty())
                m_port.value() = pair.second;

        if (m_protocol.empty())
            m_protocol = protocol_t{DEFAULT_PROTOCOL};

        if (m_port.empty())
            m_port = port_t{DEFAULT_PORT};
        
        if (m_path.empty())
            m_path = path_t(DEFAULT_PATH);
    }

    url_t uri_t::make_url() const {
        std::ostringstream out;

        if (not m_protocol.empty())
            out << m_protocol << "://";
        if (not m_domain.empty())
            out << m_domain;
        if (not m_port.empty())
            out << ":" << m_port;
        if (not m_path.empty())
            out << m_path;
        if (not m_fragment.empty())
            out << "#" << m_fragment;
        if (not m_query.empty())
            out << "?" << m_query;

        return url_t(out.str());
    }

    std::ostream& operator<<(std::ostream& out, const uri_t& uri) {
        out << uri.url() << "\n\n"
            << "protocol: " << uri.protocol() << "\n"
            << "domain: " << uri.domain() << "\n"
            << "port: " << uri.port() << "\n"
            << "path: " << uri.path() << "\n"
            << "fragment: " << uri.fragment() << "\n"
            << "query: " << uri.query() << "\n";

        return out;
    }

    uri_t operator "" _uri(const char* val, size_t) {
        return uri_t::from_string(val);
    }


} /* namespace crequests */
