#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include <sstream>

namespace requests {

    bool is_url_encoded(const string_t& value);
    string_t urlencode(const string_t& value);
    string_t urldecode(const string_t& value);
    string_t trim(const string_t& value);
    string_t tolower(const string_t& value);
    string_t toupper(const string_t& value);
    string_t compress(const string_t& value);
    string_t decompress(const string_t& value);
    string_t b64encode(const string_t& value);
    string_t b64decode(const string_t& value);
    vector_t<string_t> split(const string_t& value, const char delimiter);
    bool is_ip_address(const string_t& domain);
    string_t time_to_string(const std::time_t& time, const string_t& format);
    std::time_t string_to_time(const string_t& str, const string_t& format);
    std::time_t now_gmt();
    std::time_t max_time();

} /* namespace requests */

#endif /* UTILS_H */
