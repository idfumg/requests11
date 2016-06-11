#ifndef ASYNCRESPONSE_H
#define ASYNCRESPONSE_H

#include "types.h"
#include "response.h"

namespace crequests {


    class asyncresponse_t {
    public:
        asyncresponse_t(const future_t<response_ptr_t>& future);
        asyncresponse_t(future_t<response_ptr_t>&& future);
        asyncresponse_t(const asyncresponse_t& response);
        asyncresponse_t(asyncresponse_t&& response);
        asyncresponse_t& operator=(const asyncresponse_t& response);
        ~asyncresponse_t();

    public:
        response_ptr_t get() const;

    private:
        future_t<response_ptr_t> m_future;
    };

    using asyncresponse_ptr_t = shared_ptr_t<asyncresponse_t>;

    
} /* namespace crequests */

#endif /* ASYNCRESPONSE_H */
