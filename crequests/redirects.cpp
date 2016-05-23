#include "redirects.h"
#include "response.h"
#include <ostream>

namespace crequests {


    void redirects_t::add(const response_t& response) {
        for (auto&& resp : *this)
            if (resp.request().uri().domain() == response.request().uri().domain())
                return;
        this->push_back(response);
    }

    optional_t<response_t> redirects_t::find(const request_t& request) const {
        for (auto&& resp : *this)
            if (resp.request().uri().domain() == request.uri().domain())
                return this->back();

        return boost::none;
    }

    std::ostream& operator<<(std::ostream& out, const redirects_t& redirects) {
        for (auto&& redirect : redirects)
            out << redirect << "\n";

        return out;
    }

    
} /* namespace crequests */
