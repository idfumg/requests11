#ifndef ASYNCRESPONSE_H
#define ASYNCRESPONSE_H

#include "types.h"
#include "response.h"

namespace crequests {


    class asyncresponse_t {
    public:
        asyncresponse_t(const future_t<response_t>& future);
        asyncresponse_t(future_t<response_t>&& future);
        asyncresponse_t(const asyncresponse_t& response);
        asyncresponse_t(asyncresponse_t&& response);
        asyncresponse_t& operator=(const asyncresponse_t& response);
        ~asyncresponse_t();

    public:
        response_t get() const;

    private:
        friend class asyncrequest_impl_t;
        shared_ptr_t<class asyncrequest_impl_t> m_pimpl;
    };

    
} /* namespace crequests */

#endif /* ASYNCRESPONSE_H */
