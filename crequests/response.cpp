#include "response.h"
#include "utils.h"

namespace crequests {


    response_t::response_t(const request_t& request)
        : m_request(request)
    {
        
    }

    response_t::response_t(request_t&& request)
        : m_request(std::move(request))
    {
        
    }

    response_t::response_t(const response_t& response)
        : m_request {response.m_request},
          m_http_major {response.m_http_major},
          m_http_minor {response.m_http_minor},
          m_status_code {response.m_status_code},
          m_status_message {response.m_status_message},
          m_headers {response.m_headers},
          m_raw {response.m_raw},
          m_error {response.m_error},
          m_redirect_count {response.m_redirect_count},
          m_content {response.m_content},
          m_redirects {response.m_redirects}
    {
        
    }

    response_t::response_t(response_t&& response)
        : m_request {std::move(response.m_request)},
          m_http_major {std::move(response.m_http_major)},
          m_http_minor {std::move(response.m_http_minor)},
          m_status_code {std::move(response.m_status_code)},
          m_status_message {std::move(response.m_status_message)},
          m_headers {std::move(response.m_headers)},
          m_raw {std::move(response.m_raw)},
          m_error {std::move(response.m_error)},
          m_redirect_count {std::move(response.m_redirect_count)},
          m_content {std::move(response.m_content)},
          m_redirects {std::move(response.m_redirects)}
    {
        
    }

    response_t& response_t::operator=(const response_t& response) {
        if (this != &response) {
            m_request = response.m_request;
            m_http_major = response.m_http_major;
            m_http_minor = response.m_http_minor;
            m_status_code = response.m_status_code;
            m_status_message = response.m_status_message;
            m_headers = response.m_headers;
            m_raw = response.m_raw;
            m_error = response.m_error;
            m_redirect_count = response.m_redirect_count;
            m_content = response.m_content;
            m_redirects = response.m_redirects;
        }

        return *this;
    }

    response_t::~response_t() {
        
    }


    /****************************************************************************
     * Set. Constant reference.
     ***************************************************************************/


    void response_t::request(const request_t& request) {
        m_request = std::move(request);
    }

    void response_t::http_major(const http_major_t& http_major) {
        m_http_major = http_major;
    }

    void response_t::http_minor(const http_minor_t& http_minor) {
        m_http_minor = http_minor;
    }

    void response_t::status_code(const status_code_t& status_code) {
        m_status_code = status_code;
    }

    void response_t::status_message(const status_message_t& status_message) {
        m_status_message = status_message;
    }

    void response_t::raw(const raw_t& raw) {
        m_raw = raw;
    }

    void response_t::error(const error_t& error) {
        m_error = error;
    }

    void response_t::headers(const headers_t& headers) {
        m_headers = headers;
    }

    void response_t::redirect_count(const redirect_count_t& redirect_count) {
        m_redirect_count = redirect_count;
    }

    void response_t::content(const content_t& content) {
        m_content = content;
    }

    void response_t::redirects(const redirects_t& redirects) {
        m_redirects = redirects;
    }


    /****************************************************************************
     * Set. Rvalue reference.
     ***************************************************************************/


    void response_t::request(request_t&& request) {
        m_request = std::move(request);
    }

    void response_t::http_major(http_major_t&& http_major) {
        m_http_major = std::move(http_major);
    }

    void response_t::http_minor(http_minor_t&& http_minor) {
        m_http_minor = std::move(http_minor);
    }

    void response_t::status_code(status_code_t&& status_code) {
        m_status_code = std::move(status_code);
    }

    void response_t::status_message(status_message_t&& status_message) {
        m_status_message = std::move(status_message);
    }

    void response_t::raw(raw_t&& raw) {
        m_raw = std::move(raw);
    }

    void response_t::error(error_t&& error) {
        m_error = std::move(error);
    }

    void response_t::headers(headers_t&& headers) {
        m_headers = std::move(headers);
    }

    void response_t::redirect_count(redirect_count_t&& redirect_count) {
        m_redirect_count = std::move(redirect_count);
    }

    void response_t::content(content_t&& content) {
        m_content = std::move(content);
    }

    void response_t::redirects(redirects_t&& redirects) {
        m_redirects = std::move(redirects);
    }


    /****************************************************************************
     * Get. Constant reference.
     ***************************************************************************/


    const request_t& response_t::request() const {
        return m_request;
    }

    const http_major_t& response_t::http_major() const {
        return m_http_major;
    }

    const http_minor_t& response_t::http_minor() const {
        return m_http_minor;
    }

    const status_code_t& response_t::status_code() const {
        return m_status_code;
    }

    const status_message_t& response_t::status_message() const {
        return m_status_message;
    }

    const raw_t& response_t::raw() const {
        return m_raw;
    }

    const error_t& response_t::error() const {
        return m_error;
    }

    const headers_t& response_t::headers() const {
        return m_headers;
    }

    const redirect_count_t& response_t::redirect_count() const {
        return m_redirect_count;
    }

    const content_t& response_t::content() const {
        if (m_content.value().empty() and not m_raw.empty()) {
            if (m_headers.contains("Content-Encoding", "gzip"))
                m_content = content_t(decompress(m_raw.value()));
            else
                m_content = content_t(m_raw.value());
        }
        
        return m_content;
    }

    const redirects_t& response_t::redirects() const {
        return m_redirects;
    }


    request_t& response_t::request() {
        return m_request;
    }
    
    cookies_t& response_t::cookies() {
        return m_cookies;
    }


    /****************************************************************************
     * Other functions.
     ***************************************************************************/


    std::ostream& operator<<(std::ostream& out, const response_t& response) {
        out << "Response: "
            << "HTTP/"
            << response.http_major() << "."
            << response.http_minor() << " "
            << response.status_code() << " "
            << response.status_message() << "\n\n";

        if (not response.headers().empty())
            out << response.headers() << "\n";
        if (not response.raw().value().empty())
            out << "raw " << "(" << response.raw().value().size() << "):\n"
                << response.raw().value().substr(0, 10) << "..." << "\n"
                << "\n";

        return out;
    }


} /* namespace crequests */
