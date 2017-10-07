#ifndef REDIRECTS_H
#define REDIRECTS_H

#include "types.h"
#include <vector>

namespace crequests {


    class redirects_t {
    public:
        redirects_t();
        redirects_t(const vector_t<response_t>& responses);
        redirects_t(vector_t<response_t>&& responses);
        const vector_t<response_t>& get() const;
        vector_t<response_t>& get();
        void add(const response_t& response);
        optional_t<response_t> find(const request_t& request) const;

    private:
        vector_t<response_t> m_data;
    };

    std::ostream& operator<<(std::ostream& out, const redirects_t& redirects);


} /* namespace crequests */

#endif /* REDIRECTS_H */
