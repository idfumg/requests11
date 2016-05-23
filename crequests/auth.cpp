#include "auth.h"
#include <ostream>

namespace requests {


    auth_t auth_t::from_string(const string_t& str) {
        const auto ind = str.find(":");
        if (ind != string_t::npos)
            return
                auth_t {
                    str.substr(0, ind),
                    str.substr(ind + 1)
                };
        
        throw std::runtime_error("unexpected auth_t str: " + str);
    }
    
    string_t auth_t::to_string() {
        return this->first + ":" + this->second;
    }

    std::ostream& operator<<(std::ostream& out, const auth_t& auth) {
        out << "auth_t(" << auth.first << ", " << auth.second << ")";
        return out;
    }

    auth_t operator "" _auth(const char* val, size_t) {
        return auth_t::from_string(val);
    }


} /* namespace requests */
