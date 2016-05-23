#ifndef REDIRECTS_H
#define REDIRECTS_H

#include "types.h"
#include <vector>

namespace crequests {


    class redirects_t : public std::vector<response_t> {
        using std::vector<response_t>::vector;

    public:
        void add(const response_t& response);
        optional_t<response_t> find(const request_t& request) const;
    };

    std::ostream& operator<<(std::ostream& out, const redirects_t& redirects);


} /* namespace crequests */

#endif /* REDIRECTS_H */
