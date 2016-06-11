#include "ssl_auth.h"
#include <sstream>

namespace crequests {

    
    string_t ssl_auth_t::to_string() const {
        std::ostringstream out;
        out << this->first << this->second;
        return out.str();
    }

    std::ostream& operator<<(std::ostream& out, const ssl_auth_t& auth) {
        out << "ssl_auth_t(" << auth.first << ", " << auth.second << ")";
        return out;
    }


} /* namespace crequests */
