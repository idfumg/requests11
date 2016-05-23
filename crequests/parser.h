#ifndef PARSER_H
#define PARSER_H

#include "../external/http_parser/http_parser.h"
#include <string>
#include <functional>

namespace crequests {

    
    using cb_data_t = std::function<void(const char* at, size_t length)>;
    using cb_info_t = std::function<void()>;
    using cb_chunk_header_t = std::function<void(size_t length)>;
    using cb_status_t =
        std::function<void(const char* status,
                           size_t length,
                           unsigned short http_major,
                           unsigned short http_minor,
                           unsigned int status_code)>;

    
    class parser_t {
    public:
        enum class parser_type_t {
            REQUEST,
            RESPONSE
        };
        
        enum cb_type_t {
            MESSAGE_BEGIN,
            URL,
            STATUS,
            HEADER_FIELD,
            HEADER_VALUE,
            HEADERS_COMPLETE,
            BODY,
            MESSAGE_COMPLETE,
            CHUNK_HEADER,
            CHUNK_COMPLETE
        };
        
    public:
        parser_t(const parser_type_t& parser_type);
        ~parser_t();

    public:
        size_t execute(const char* data, size_t len);
        void bind_cb(const cb_type_t& type, cb_data_t fn);
        void bind_cb(const cb_type_t& type, cb_info_t fn);
        void bind_cb(cb_status_t fn);
        void bind_cb(const cb_type_t& type, cb_chunk_header_t fn);
        void pause();
        void unpause();

    private:
        http_parser parser;
        http_parser_settings settings;

    public:
        struct data_t {
            cb_info_t on_message_begin {nullptr};
            cb_data_t on_url {nullptr};
            cb_status_t on_status {nullptr};
            cb_data_t on_header_field {nullptr};
            cb_data_t on_header_value {nullptr};
            cb_chunk_header_t on_headers_complete {nullptr};
            cb_data_t on_body {nullptr};
            cb_info_t on_message_complete {nullptr};
            cb_chunk_header_t on_chunk_header {nullptr};
            cb_info_t on_chunk_complete {nullptr};
            
            bool m_need_pausing {false};
        } data {};
    };

    
} /* namespace crequests */

#endif /* PARSER_H */
