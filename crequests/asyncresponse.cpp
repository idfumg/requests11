#include "asyncresponse.h"

namespace crequests {
    

    asyncresponse_t::asyncresponse_t(const future_t<response_ptr_t>& future)
        : m_future(future)
    {
    }
    
    asyncresponse_t::asyncresponse_t(future_t<response_ptr_t>&& future)
        : m_future(std::move(future))
    {
    }
    
    asyncresponse_t::asyncresponse_t(const asyncresponse_t& response)
        : m_future {response.m_future}
    {
    }
    
    asyncresponse_t::asyncresponse_t(asyncresponse_t&& response)
        : m_future {std::move(response.m_future)}
    {
    }
    
    asyncresponse_t& asyncresponse_t::operator=(const asyncresponse_t& response) {
        if (this != &response) {
            m_future = response.m_future;
        }

        return *this;
    }
    
    asyncresponse_t::~asyncresponse_t() {
    }


    /****************************************************************************
     * Other functions.
     ***************************************************************************/

    response_ptr_t asyncresponse_t::get() const {
        return m_future.get();
    }


} /* namespace crequests */
