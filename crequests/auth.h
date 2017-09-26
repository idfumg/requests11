#ifndef AUTH_H
#define AUTH_H

#include "types.h"
#include "macros.h"

namespace crequests {

    declare_string(login)
    declare_string(password)
    
    using pair_t = std::pair<login_t, password_t>;

    class auth_t : public pair_t {
        using pair_t::pair_t;

    public:
        static auth_t from_string(const string_t& str);
        string_t to_string() const;
    };

    std::ostream& operator<<(std::ostream& out, const auth_t& auth);
    auth_t operator "" _auth(const char* val, size_t);

} /* namespace crequests */

#endif /* AUTH_H */
