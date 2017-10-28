#include "parser.h"
#include <iostream>
#include <string.h>

namespace crequests {

    
    namespace {

        
        int cb_message_begin(http_parser* parser) {
            const auto data = static_cast<parser_t::data_t*>(parser->data);
            if (data->on_message_begin)
                data->on_message_begin();

            return 0;
        }

        int cb_url(http_parser* parser, const char* at, size_t length) {
            const auto data = static_cast<parser_t::data_t*>(parser->data);
            if (data->on_url)
                data->on_url(at, length);
            
            return 0;
        }
    
        int cb_status(http_parser* parser, const char* at, size_t length) {
            const auto data = static_cast<parser_t::data_t*>(parser->data);
            if (data->on_status)
                data->on_status(at,
                                length,
                                parser->http_major,
                                parser->http_minor,
                                parser->status_code);
            
            return 0;
        }

        int cb_header_field(http_parser* parser, const char* at, size_t length) {
            const auto data = static_cast<parser_t::data_t*>(parser->data);
            if (data->on_header_field)
                data->on_header_field(at, length);

            return 0;
        }

        int cb_header_value(http_parser* parser, const char* at, size_t length) {
            const auto data = static_cast<parser_t::data_t*>(parser->data);
            if (data->on_header_value)
                data->on_header_value(at, length);

            return 0;
        }

        int cb_headers_complete(http_parser* parser) {
            const auto data = static_cast<parser_t::data_t*>(parser->data);
            if (data->on_headers_complete)
                data->on_headers_complete(parser->content_length);
            
            return 0;
        }

        int cb_body(http_parser* parser, const char* at, size_t length) {
            const auto data = static_cast<parser_t::data_t*>(parser->data);
            if (data->on_body)
                data->on_body(at, length);
            
            return 0;
        }

        int cb_message_complete(http_parser* parser) {
            const auto data = static_cast<parser_t::data_t*>(parser->data);
            if (data->on_message_complete)
                data->on_message_complete();
            
            return 0;
        }

        int cb_chunk_header(http_parser* parser) {
            const auto data = static_cast<parser_t::data_t*>(parser->data);
            if (data->on_chunk_header)
                data->on_chunk_header(parser->content_length);
            
            return 0;
        }

        int cb_chunk_complete(http_parser* parser) {
            const auto data = static_cast<parser_t::data_t*>(parser->data);
            if (data->on_chunk_complete)
                data->on_chunk_complete();

            return 0;
        }

        
    } /* anonymous namespace */

    
    parser_t::parser_t(const parser_type_t& parser_type) {
        http_parser_init(
            &parser,
            parser_type == parser_type_t::REQUEST ? HTTP_REQUEST : HTTP_RESPONSE);
        data = data_t{};
        parser.data = &data;
        
        http_parser_settings_init(&settings);
        settings.on_message_begin = cb_message_begin;
        settings.on_url = cb_url;
        settings.on_status = cb_status;
        settings.on_header_field = cb_header_field;
        settings.on_header_value = cb_header_value;
        settings.on_headers_complete = cb_headers_complete;
        settings.on_body = cb_body;
        settings.on_chunk_header = cb_chunk_header;
        settings.on_chunk_complete = cb_chunk_complete;
        settings.on_message_complete = cb_message_complete;
    }

    parser_t::~parser_t() {

    }

    void parser_t::pause() {
        if (parser.http_errno != HPE_PAUSED)
            http_parser_pause(&parser, 1);
    }

    void parser_t::unpause() {
        if (parser.http_errno == HPE_PAUSED)
            http_parser_pause(&parser, 0);
    }

    size_t parser_t::execute(const char* data_, size_t length) {
        const size_t nparsed = http_parser_execute(&parser, &settings, data_, length);
        if (parser.http_errno != HPE_OK and parser.http_errno != HPE_PAUSED)
            return 0;
        return nparsed;
    }

    void parser_t::bind_cb(const cb_type_t& type, cb_data_t fn) {
        switch (type) {
        case MESSAGE_BEGIN:
            break;
        case URL:
            data.on_url = fn;
            break;
        case STATUS:
            break;
        case HEADER_FIELD:
            data.on_header_field = fn;
            break;
        case HEADER_VALUE:
            data.on_header_value = fn;
            break;
        case HEADERS_COMPLETE:
            break;
        case BODY:
            data.on_body = fn;
            break;
        case MESSAGE_COMPLETE:
            break;
        case CHUNK_HEADER:
            break;
        case CHUNK_COMPLETE:
            break;
        }
    }

    void parser_t::bind_cb(const cb_type_t& type, cb_info_t fn) {
        switch (type) {
        case MESSAGE_BEGIN:
            data.on_message_begin = fn;
            break;
        case URL:
            break;
        case STATUS:
            break;
        case HEADER_FIELD:
            break;
        case HEADER_VALUE:
            break;
        case HEADERS_COMPLETE:
            break;
        case BODY:
            break;
        case MESSAGE_COMPLETE:
            data.on_message_complete = fn;
        case CHUNK_HEADER:
            break;
        case CHUNK_COMPLETE:
            data.on_chunk_complete = fn;
        }
    }

    void parser_t::bind_cb(cb_status_t fn) {
        data.on_status = fn;
    }

    void parser_t::bind_cb(const cb_type_t& type, cb_chunk_header_t fn) {
        if (type == CHUNK_HEADER)
            data.on_chunk_header = fn;
        else if (type == HEADERS_COMPLETE)
            data.on_headers_complete = fn;
    }
    
    
} /* namespace crequests */
