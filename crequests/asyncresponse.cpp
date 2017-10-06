#include "asyncresponse.h"

namespace crequests {


    struct asyncrequest_impl_t {
    public:
        asyncrequest_impl_t(const future_t<response_t>& future)
            : m_future{future}
        {

        }

        asyncrequest_impl_t(future_t<response_t>&& future)
            : m_future{std::move(future)}
        {

        }

    public:
        future_t<response_t> m_future;
    };    

    asyncresponse_t::asyncresponse_t(const future_t<response_t>& future)
        : m_pimpl{std::make_shared<asyncrequest_impl_t>(future)}
    {
    }
    
    asyncresponse_t::asyncresponse_t(future_t<response_t>&& future)
        : m_pimpl{std::make_shared<asyncrequest_impl_t>(std::move(future))}
    {
        
    }
    
    asyncresponse_t::asyncresponse_t(const asyncresponse_t& response)
        : m_pimpl{response.m_pimpl}
    {
    }
    
    asyncresponse_t::asyncresponse_t(asyncresponse_t&& response)
        : m_pimpl{std::move(response.m_pimpl)}
    {
    }
    
    asyncresponse_t& asyncresponse_t::operator=(const asyncresponse_t& response)
    {
        if (this != &response) {
            m_pimpl = response.m_pimpl;
        }

        return *this;
    }
    
    asyncresponse_t::~asyncresponse_t()
    {
        
    }


    /****************************************************************************
     * Other functions.
     ***************************************************************************/

    response_t asyncresponse_t::get() const
    {
        return m_pimpl->m_future.get();
    }


} /* namespace crequests */
