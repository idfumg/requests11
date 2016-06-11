#ifndef SSL_CERTS_H
#define SSL_CERTS_H

#include "types.h"
#include "ssl_auth.h"
#include <vector>

namespace crequests {


    class ssl_certs_t : public vector_t<certificate_t> {
        using vector_t<certificate_t>::vector;

    public:
        void add(const certificate_t& cert);
    };

    std::ostream& operator<<(std::ostream& out, const ssl_certs_t& ssl_certs);


} /* namespace crequests */

#endif /* SSL_CERTS_H */
