#ifndef HEADER_H
#define HEADER_H

#include "types.h"

namespace requests {

    using cookie_map = std::unordered_multimap<string_t, string_t, ihash, iequals>;

    class headers_t : public cookie_map {
        using cookie_map::cookie_map;

    public:
        static headers_t from_string(const string_t& str);
        string_t to_string() const;
        void update(const headers_t& params);
        bool contains(const string_t& name, const string_t& value) const;
        void insert(const string_t& name, const string_t& value);
        string_t at(const string_t& name) const;
    };

    std::ostream& operator<<(std::ostream& out, const headers_t& headers);
    headers_t operator "" _headers(const char* val, size_t);

} /* namespace requests */

#endif /* HEADER_H */
