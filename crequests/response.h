#ifndef RESPONSE_H
#define RESPONSE_H

#include "macros.h"
#include "error.h"
#include "headers.h"
#include "request.h"
#include "redirects.h"
#include "cookies.h"

namespace crequests {

    declare_number(http_major, unsigned short)
    declare_number(http_minor, unsigned short)
    declare_number(status_code, unsigned int)
    declare_string(status_message)
    declare_string(raw)
    declare_string(content)
    
    class response_t {
    public:
        response_t(const request_t& request);
        response_t(request_t&& request);
        response_t(const response_t& response);
        response_t(response_t&& response);
        response_t& operator=(const response_t& response);
        response_t& operator=(response_t&& response);
        ~response_t();

    public:
        void request(const request_t& request);
        void http_major(const http_major_t& http_major);
        void http_minor(const http_minor_t& http_minor);
        void status_code(const status_code_t& status_code);
        void status_message(const status_message_t& status_message);
        void raw(const raw_t& raw);
        void error(const error_t& error);
        void headers(const headers_t& headers);
        void redirect_count(const redirect_count_t& redirect_count);
        void content(const content_t& content);
        void redirects(const redirects_t& redirects);
        void cookies(const cookies_t& cookies);

        void request(request_t&& request);
        void http_major(http_major_t&& http_major);
        void http_minor(http_minor_t&& http_minor);
        void status_code(status_code_t&& status_code);
        void status_message(status_message_t&& status_message);
        void raw(raw_t&& raw);
        void error(error_t&& error);
        void headers(headers_t&& headers);
        void redirect_count(redirect_count_t&& redirect_count);
        void content(content_t&& content);
        void redirects(redirects_t&& redirects);
        void cookies(cookies_t&& cookies);

        const request_t& request() const;
        const http_major_t& http_major() const;
        const http_minor_t& http_minor() const;
        const status_code_t& status_code() const;
        const status_message_t& status_message() const;
        const raw_t& raw() const;
        const error_t& error() const;
        const headers_t& headers() const;
        const redirect_count_t& redirect_count() const;
        const content_t& content() const;
        const redirects_t& redirects() const;
        const cookies_t& cookies() const;

        request_t& request();
        cookies_t& cookies();

    private:
        friend class response_impl_t;
        shared_ptr_t<class response_impl_t> m_pimpl;
    };

    std::ostream& operator<<(std::ostream& out, const response_t& response);
    
} /* namespace crequests */

#endif /* RESPONSE_H */
