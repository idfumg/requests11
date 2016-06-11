#include "ssl_certs.h"
#include <ostream>

namespace crequests {


    void ssl_certs_t::add(const certificate_t& cert) {
        this->push_back(cert);
    }

    std::ostream& operator<<(std::ostream& out, const ssl_certs_t& ssl_certs) {
        for (auto&& cert : ssl_certs)
            out << cert << "\n";

        return out;
    }

    
} /* namespace crequests */
