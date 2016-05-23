#ifndef AUTH_H
#define AUTH_H

#include "types.h"

namespace crequests {

    using pair_t = std::pair<string_t, string_t>;

    class auth_t : public pair_t {
        using pair_t::pair_t;

    public:
        static auth_t from_string(const string_t& str);
        string_t to_string();
    };

    std::ostream& operator<<(std::ostream& out, const auth_t& auth);
    auth_t operator "" _auth(const char* val, size_t);

} /* namespace crequests */

#endif /* AUTH_H */
