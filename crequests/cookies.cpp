#include "cookies.h"
#include "utils.h"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <string.h>

namespace crequests {


    /************************************************************
     * cookie_t implementation.
     ************************************************************/


    cookie_t::cookie_t()
        : m_name{},
          m_value{},
          m_domain{},
          m_path{},
          m_origin_domain{},
          m_origin_path{},
          m_is_secure{false},
          m_is_http_only{false},
          m_expires{}
    {

    }

    cookie_t::cookie_t(const cookie_t& cookie)
        : m_name(cookie.m_name),
          m_value(cookie.m_value),
          m_domain(cookie.m_domain),
          m_path(cookie.m_path),
          m_origin_domain(cookie.m_origin_domain),
          m_origin_path(cookie.m_origin_path),
          m_is_secure(cookie.m_is_secure),
          m_is_http_only(cookie.m_is_http_only),
          m_expires(cookie.m_expires)
    {

    }

    cookie_t::cookie_t(cookie_t&& cookie)
        : m_name(std::move(cookie.m_name)),
          m_value(std::move(cookie.m_value)),
          m_domain(std::move(cookie.m_domain)),
          m_path(std::move(cookie.m_path)),
          m_origin_domain(std::move(cookie.m_origin_domain)),
          m_origin_path(std::move(cookie.m_origin_path)),
          m_is_secure(std::move(cookie.m_is_secure)),
          m_is_http_only(std::move(cookie.m_is_http_only)),
          m_expires(std::move(cookie.m_expires))
    {

    }

    cookie_t& cookie_t::operator=(const cookie_t& cookie) {
        if (this != &cookie) {
            m_name = cookie.m_name;
            m_value = cookie.m_value;
            m_domain = cookie.m_domain;
            m_path = cookie.m_path;
            m_origin_domain = cookie.m_origin_domain;
            m_origin_path = cookie.m_origin_path;
            m_is_secure = cookie.m_is_secure;
            m_is_http_only = cookie.m_is_http_only;
            m_expires = cookie.m_expires;
        }

        return *this;
    }

    cookie_t& cookie_t::operator=(cookie_t&& cookie) {
        if (this != &cookie) {
            m_name = std::move(cookie.m_name);
            m_value = std::move(cookie.m_value);
            m_domain = std::move(cookie.m_domain);
            m_path = std::move(cookie.m_path);
            m_origin_domain = std::move(cookie.m_origin_domain);
            m_origin_path = std::move(cookie.m_origin_path);
            m_is_secure = std::move(cookie.m_is_secure);
            m_is_http_only = std::move(cookie.m_is_http_only);
            m_expires = std::move(cookie.m_expires);
        }

        return *this;
    }

    cookie_t::cookie_t(const string_t& origin_domain,
                       const string_t& origin_path,
                       const string_t& cookie_text) {
        auto cookie = cookie_t::from_string(cookie_text);
        cookie.origin_domain(origin_domain);
        cookie.origin_path(origin_path);
        *this = std::move(cookie);
    }

    void cookie_t::name(const string_t& str) {
        m_name = str;
    }

    void cookie_t::value(const string_t& str) {
        m_value = str;
    }

    void cookie_t::expires(const std::time_t& time) {
        m_expires = time;
    }

    void cookie_t::path(const string_t& str) {
        m_path = str;
    }

    void cookie_t::domain(const string_t& str) {
        m_domain = str;
    }

    void cookie_t::origin_path(const string_t& str) {
        m_origin_path = str;
    }

    void cookie_t::origin_domain(const string_t& str) {
        m_origin_domain = str;
    }

    void cookie_t::is_secure(bool val) {
        m_is_secure = val;
    }

    void cookie_t::is_http_only(bool val) {
        m_is_http_only = val;
    }

    void cookie_t::name(string_t&& str) {
        m_name = std::move(str);
    }

    void cookie_t::value(string_t&& str) {
        m_value = std::move(str);
    }

    void cookie_t::path(string_t&& str) {
        m_path = std::move(str);
    }

    void cookie_t::domain(string_t&& str) {
        m_domain = std::move(str);
    }

    void cookie_t::origin_path(string_t&& str) {
        m_origin_path = std::move(str);
    }

    void cookie_t::origin_domain(string_t&& str) {
        m_origin_domain = std::move(str);
    }

    const string_t& cookie_t::name() const {
        return m_name;
    }

    const string_t& cookie_t::value() const {
        return m_value;
    }

    const string_t& cookie_t::path() const {
        return m_path;
    }

    const string_t& cookie_t::domain() const {
        return m_domain;
    }

    const string_t& cookie_t::origin_path() const {
        return m_origin_path;
    }

    const string_t& cookie_t::origin_domain() const {
        return m_origin_domain;
    }

    const std::time_t& cookie_t::expires() const {
        return m_expires;
    }

    bool cookie_t::is_secure() const {
        return m_is_secure;
    }

    bool cookie_t::is_http_only() const {
        return m_is_http_only;
    }

    cookie_t cookie_t::from_string(const string_t& str) {
        cookie_t cookie;

        std::istringstream in(str);
        string_t token;
        while (std::getline(in, token, ';')) {
            token = trim(token);
            if (token.empty()) {
                break;
            }

            const auto ind = token.find("=");
            if (ind != string_t::npos) {
                auto name = token.substr(0, ind);
                auto value = token.substr(ind + 1);
                const auto name_lower = tolower(name);

                if (name_lower == "expires") {
                    cookie.expires(string_to_time(value, "%a, %d %b %Y %H:%M:%S %Z"));
                }
                else if (name_lower == "path") {
                    cookie.path(std::move(value));
                }
                else if (name_lower == "domain") {
                    if (value.at(0) == '.') {
                        value.replace(0, 1, "");
                    }
                    cookie.domain(std::move(value));
                }
                else {
                    cookie.name(std::move(name));
                    cookie.value(std::move(value));
                }
            }
            else {
                const auto token_lower = tolower(token);
                if (token_lower == "secure") {
                    cookie.is_secure(true);
                }
                else if (token_lower == "httponly") {
                    cookie.is_http_only(true);
                }
                else {
                    cookie.name(std::move(token));
                }
            }
        }

        return cookie;
    }

    string_t cookie_t::to_string() const {
        std::ostringstream out;

        out << m_name;
        if (not m_value.empty())
            out << "=" << m_value;
        if (m_expires)
            out << "; Expires="
                << time_to_string(m_expires, "%a, %d %b %Y %H:%M:%S %Z");
        if (m_is_secure)
            out << "; Secure";
        if (m_is_http_only)
            out << "; HttpOnly";
        if (not m_path.empty())
            out << "; Path=" << m_path;
        if (not m_domain.empty())
            out << "; Domain=" << m_domain;

        return out.str();
    }

    bool operator<(const cookie_t& first, const cookie_t& second) {
        const auto first_domain =
            first.domain().empty() ? first.origin_domain() : first.domain();
        const auto second_domain =
            second.domain().empty() ? second.origin_domain() : second.domain();

        const auto first_path =
            first.path().empty() ? first.origin_path() : first.path();
        const auto second_path =
            second.path().empty() ? second.origin_path() : second.path();

        return
            first.name() < second.name() or
            first_domain < second_domain or
            first_path < second_path;
    }

    std::ostream& operator<<(std::ostream& out, const cookie_t& cookie) {
        out << cookie.to_string();
        return out;
    }


    /************************************************************
     * cookies_list_t implementation.
     ************************************************************/


    void cookies_list_t::add(const cookie_t& cookie) {
        const auto it = insert(cookie);
        if (not it.second) {
            erase(it.first);
            insert(cookie);
        }
    }

    string_t cookies_list_t::to_string() const {
        std::map<string_t, string_t> map;

        for (const auto& cookie : *this) {
            map[cookie.name()] = cookie.value();
        }

        std::ostringstream out;
        for (const auto& cookie : map) {
            out << cookie.first;
            if (not cookie.second.empty()) {
                out << "=" << cookie.second;
            }
            out << "; ";
        }

        return out.str();
    }


    /************************************************************
     * cookies_t implementation.
     ************************************************************/


    namespace {


        bool is_sibling_domain(const int distance) {
            return distance == 0;
        }

        bool is_child_domain(const int distance) {
            return distance > 0;
        }

        bool is_parent_domain(const int distance) {
            return distance < 0;
        }

        bool is_public_domain(const string_t& domains) {
            /* public domain suffixes on: https://publicsuffix.org */
            static const string_t public_domain = "compute.amazonaws.com";

            return
                is_ip_address(domains) or
                std::count(domains.begin(), domains.end(), '.') == 0 or
                public_domain == domains;
        }

        bool is_public_domain_when_walk_through_parents(string_t& origin,
                                                        size_t distance) {
            for (size_t i = 0; i < distance; ++i) {
                if (is_public_domain(origin)) {
                    return true;
                }

                const auto ind = origin.find(".");
                if (ind != string_t::npos) {
                    origin.replace(0, ind + 1, "");
                }
            }

            return origin.find(".") == string_t::npos;
        }

        bool is_domain_cover(const string_t& domains, string_t origin, size_t distance) {
            if (is_public_domain(domains))
                return false;

            if (is_public_domain_when_walk_through_parents(origin, distance))
                return false;

            return domains == origin;
        }

        bool is_domain_matched(const string_t& domains, const string_t& origin) {
            const int distance =
                std::count(domains.begin(), domains.end(), '.') -
                std::count(origin.begin(), origin.end(), '.');

            if (is_sibling_domain(distance))
                return domains == origin;

            if (is_parent_domain(distance))
                return is_domain_cover(domains, origin, std::abs(distance));

            if (is_child_domain(distance))
                return is_domain_cover(origin, domains, std::abs(distance));

            return true;
        }

        bool is_path_matched(const string_t& cookie_path,
                             const string_t& req_path) {
            // RFC6265 5.1.4
            const auto is_equal =
                cookie_path.compare(0, cookie_path.size(), req_path);

            if (is_equal == 0)
                return true;

            if (is_equal > 0)
                return false;

            // its not a substring
            if (std::abs(is_equal) > static_cast<int>(req_path.size()))
                return false;

            if (cookie_path.back() == '/')
                return true;

            if ((req_path.size() > cookie_path.size() && req_path.at(cookie_path.size()) == '/') ||
                (cookie_path.size() == req_path.size() && req_path.at(cookie_path.size()-1) == '/'))
                return true;

            return false;
        }

        bool is_cookie_expired(const cookie_t& cookie) {
            return
                now_gmt() > (cookie.expires() != 0 ? cookie.expires() : max_time());
        }

        void cut_head_and_dot(string_t& value) {
            const auto ind = value.find(".");
            if (ind != string_t::npos) {
                value.replace(0, ind + 1, "");
            }
        }


    } /* anonymous namespace */


    void cookies_t::add(const cookie_t& cookie) {
        const auto cookie_domain =
            tolower(
                cookie.domain().empty() ? cookie.origin_domain() : cookie.domain());

        if (is_domain_matched(cookie_domain, tolower(cookie.origin_domain()))) {
            m_cookies_map[cookie_domain].add(std::move(cookie));
        }
    }

    cookies_list_t cookies_t::get_inner(const string_t& domain,
                                        const string_t& path,
                                        const bool is_ssl) const {
        cookies_list_t cookies;
        cookies_list_t cookies_rm;

        if (not m_cookies_map.count(domain))
            return {};

        auto& domain_cookies = m_cookies_map[domain];

        for (const auto& cookie : domain_cookies) {
            const auto& cookie_path =
                cookie.path().empty()? cookie.origin_path() : cookie.path();

            if (not is_path_matched(cookie_path, path))
                continue;

            if (cookie.is_secure() and not is_ssl)
                continue;

            if (is_cookie_expired(cookie))
                cookies_rm.insert(cookie);
            else
                cookies.insert(cookie);
        }

        for (const auto& cookie : cookies_rm) {
            domain_cookies.erase(cookie);
        }

        return cookies;
    }

    cookies_list_t cookies_t::get(const string_t& domain_,
                                  const string_t& path,
                                  const bool is_ssl) const {
        cookies_list_t cookies;
        string_t domain = tolower(domain_);

        for (;;) {
            for (const auto& cookie : get_inner(domain, path, is_ssl)) {
                cookies.insert(cookie);
            }

            if (is_public_domain(domain)) {
                break;
            }

            cut_head_and_dot(domain);
        }

        return cookies;
    }

    string_t cookies_t::to_string() const {
        std::ostringstream out;

        for (const auto& domain_cookies : m_cookies_map) {
            for (const auto& cookie : domain_cookies.second) {
                out << cookie << "\n";
            }
            out << "\n";
        }

        return out.str();
    }

    void cookies_t::update(const cookies_t& cookies) {
        for (const auto& param : cookies.m_cookies_map) {
            m_cookies_map[param.first] = param.second;
        }
    }

    size_t cookies_t::size() const {
        return m_cookies_map.size();
    }

    std::ostream& operator<<(std::ostream& out, const cookies_t& cookies) {
        out << cookies.to_string();
        return out;
    }

    cookie_t operator "" _cookie(const char* val, size_t) {
        return cookie_t::from_string(val);
    }


} /* namespace crequests */
