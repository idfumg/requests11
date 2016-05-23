#ifndef ERROR_H
#define ERROR_H

#include "types.h"

namespace crequests {

    enum error_code_t {
        INIT,
        RESOLVE,
        RESOLVE_ERROR,
        CONNECT,
        CONNECT_ERROR,
        HANDSHAKE,
        HANDSHAKE_ERROR,
        WRITE,
        WRITE_ERROR,
        READ_STATUS,
        READ_STATUS_ERROR,
        READ_STATUS_DATA_ERROR,
        READ_HEADERS,
        READ_HEADERS_ERROR,
        READ_CONTENT_LENGTH,
        READ_CONTENT_LENGTH_ERROR,
        READ_CHUNK_HEADER,
        READ_CHUNK_HEADER_ERROR,
        READ_CHUNK_DATA,
        READ_CHUNK_DATA_ERROR,
        READ_UNTIL_EOF,
        READ_UNTIL_EOF_ERROR,
        REDIRECT_EXHAUSTED,
        REDIRECT_ERROR,
        TIMEOUT,
        EXPIRED,
        SUCCESS
    };

    string_t error_code_to_string(const error_code_t& code);

    class error_t {
    public:
        error_t();

        template <class ErrorCodeT>
        error_t(const error_code_t& error_code, ErrorCodeT&& ec)
            : error_code(error_code), msg(ec) {}

        operator bool() const;
        error_code_t code() const;
        string_t code_to_string() const;
        string_t message() const;
        string_t to_string() const;

    private:
        error_code_t error_code;
        string_t msg;
    };

    std::ostream& operator<<(std::ostream& out, const error_t& error);
    
} /* namespace crequests */

#endif /* ERROR_H */
