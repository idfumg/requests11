#include "error.h"

namespace crequests {


    string_t error_code_to_string(const error_code_t& code) {
        switch (static_cast<int>(code)) {
        case error_code_t::INIT:
            return "INIT";
        case error_code_t::RESOLVE:
            return "RESOLVE";
        case error_code_t::RESOLVE_ERROR:
            return "RESOLVE_ERROR";
        case error_code_t::CONNECT:
            return "CONNECT";
        case error_code_t::CONNECT_ERROR:
            return "CONNECT_ERROR";
        case error_code_t::HANDSHAKE:
            return "HANDSHAKE";
        case error_code_t::HANDSHAKE_ERROR:
            return "HANDSHAKE_ERROR";
        case error_code_t::WRITE:
            return "WRITE";
        case error_code_t::WRITE_ERROR:
            return "WRITE_ERROR";
        case error_code_t::READ_STATUS:
            return "READ_STATUS";
        case error_code_t::READ_STATUS_ERROR:
            return "READ_STATUS_ERROR";
        case error_code_t::READ_STATUS_DATA_ERROR:
            return "READ_STATUS_DATA_ERROR";
        case error_code_t::READ_HEADERS:
            return "READ_HEADERS";
        case error_code_t::READ_HEADERS_ERROR:
            return "READ_HEADERS_ERROR";
        case error_code_t::READ_CONTENT_LENGTH:
            return "READ_CONTENT_LENGTH";
        case error_code_t::READ_CONTENT_LENGTH_ERROR:
            return "READ_CONTENT_LENGTH_ERROR";
        case error_code_t::READ_CHUNK_HEADER:
            return "READ_CHUNK_HEADER";
        case error_code_t::READ_CHUNK_HEADER_ERROR:
            return "READ_CHUNK_HEADER_ERROR";
        case error_code_t::READ_CHUNK_DATA:
            return "READ_CHUNK_DATA";
        case error_code_t::READ_CHUNK_DATA_ERROR:
            return "READ_CHUNK_DATA_ERROR";
        case error_code_t::READ_UNTIL_EOF:
            return "READ_UNTIL_EOF";
        case error_code_t::READ_UNTIL_EOF_ERROR:
            return "READ_UNTIL_EOF_ERROR";
        case error_code_t::REDIRECT_EXHAUSTED:
            return "REDIRECT_EXHAUSTED";
        case error_code_t::REDIRECT_ERROR:
            return "REDIRECT_ERROR";
        case error_code_t::TIMEOUT:
            return "TIMEOUT";
        case error_code_t::EXPIRED:
            return "EXPIRED";
        case error_code_t::SUCCESS:
            return "SUCCESS";
        }
        return "Unexpected state = " +
            std::to_string(static_cast<int>(code));
    }
    
    error_t::error_t()
        : error_code(error_code_t::SUCCESS), msg("success")
    {}

    error_t::operator bool() const {
        return error_code != error_code_t::SUCCESS;
    }
    
    error_code_t error_t::code() const {
        return error_code;
    }

    string_t error_t::code_to_string() const {
        return error_code_to_string(error_code);
    }

    string_t error_t::message() const {
        return msg;
    }

    string_t error_t::to_string() const {
        return code_to_string() + ": " + message();
    }

    std::ostream& operator<<(std::ostream& out, const error_t& error) {
        out << error.to_string();
        return out;
    }


} /* namespace crequests */
