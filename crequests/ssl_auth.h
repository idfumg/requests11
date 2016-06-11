#ifndef SSL_AUTH_H
#define SSL_AUTH_H

#include "types.h"
#include "macros.h"
#include <vector>

namespace crequests {

    declare_string(certificate);
    declare_string(privatekey);

    using cert_key_t = std::pair<certificate_t, privatekey_t>;

    class ssl_auth_t : public cert_key_t {
        using cert_key_t::cert_key_t;

    public:
        string_t to_string() const;
    };

    std::ostream& operator<<(std::ostream& out, const ssl_auth_t& redirects);


} /* namespace crequests */

#endif /* SSL_AUTH_H */
