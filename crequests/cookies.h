#ifndef COOKIES_H
#define COOKIES_H

#include "types.h"
#include <set>

namespace crequests {


    class cookie_t {
    public:
        cookie_t();
        cookie_t(const cookie_t& cookie);
        cookie_t(cookie_t&& cookie);
        cookie_t(const string_t& origin_domain,
                 const string_t& origin_path,
                 const string_t& cookie_text);
        cookie_t& operator=(const cookie_t& cookie);
        cookie_t& operator=(cookie_t&& cookie);

    public:
        void name(const string_t& str);
        void value(const string_t& str);
        void expires(const std::time_t& time);
        void path(const string_t& str);
        void domain(const string_t& str);
        void origin_path(const string_t& str);
        void origin_domain(const string_t& str);
        void is_secure(bool val);
        void is_http_only(bool val);

        void name(string_t&& str);
        void value(string_t&& str);
        void path(string_t&& str);
        void domain(string_t&& str);
        void origin_path(string_t&& str);
        void origin_domain(string_t&& str);

        const string_t& name() const;
        const string_t& value() const;
        const string_t& path() const;
        const string_t& domain() const;
        const string_t& origin_path() const;
        const string_t& origin_domain() const;
        const time_t& expires() const;
        bool is_secure() const;
        bool is_http_only() const;

        static cookie_t from_string(const string_t& str);
        string_t to_string() const;

    private:
        string_t m_name {};
        string_t m_value {};
        string_t m_domain {};
        string_t m_path {};
        string_t m_origin_domain {};
        string_t m_origin_path {};
        bool m_is_secure {};
        bool m_is_http_only {};
        std::time_t m_expires {};
    };


    using cookies_list = std::set<cookie_t>;

    class cookies_list_t : public cookies_list {
        using cookies_list::cookies_list;

    public:
        void add(const cookie_t& cookie);
        string_t to_string() const;
    };


    class cookies_t {
    public:
        void add(const cookie_t& cookie);
        string_t to_string() const;
        void update(const cookies_t& cookies);
        cookies_list_t get(const string_t& domain,
                           const string_t& path = "/",
                           const bool is_ssl = false) const;
        size_t size() const;

    private:
        cookies_list_t get_inner(const string_t& domain,
                                 const string_t& path = "/",
                                 const bool is_ssl = false) const;

    private:
        using cookies_map =
            std::unordered_map<string_t, cookies_list_t, ihash, iequals>;
        mutable cookies_map m_cookies_map {};
    };


    bool operator<(const cookie_t& first, const cookie_t& second);
    std::ostream& operator<<(std::ostream& out, const cookie_t& cookie);
    std::ostream& operator<<(std::ostream& out, const cookies_t& cookies);
    cookie_t operator "" _cookie(const char* val, size_t);


} /* namespace crequests */

#endif /* COOKIES_H */
