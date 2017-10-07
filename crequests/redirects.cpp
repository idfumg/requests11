#include "redirects.h"
#include "response.h"
#include <ostream>

namespace crequests {


    redirects_t::redirects_t()
        : m_data{}
    {

    }

    redirects_t::redirects_t(const vector_t<response_t>& responses)
        : m_data{responses}
    {

    }

    redirects_t::redirects_t(vector_t<response_t>&& responses)
        : m_data{std::move(responses)}
    {

    }

    const vector_t<response_t>& redirects_t::get() const
    {
        return m_data;
    }

    vector_t<response_t>& redirects_t::get()
    {
        return m_data;
    }

    void redirects_t::add(const response_t& response)
    {
        for (const auto& resp : get()) {
            if (resp.request().uri().domain() == response.request().uri().domain()) {
                return;
            }
        }
        get().push_back(response);
    }

    optional_t<response_t> redirects_t::find(const request_t& request) const
    {
        for (const auto& resp : get()) {
            if (resp.request().uri().domain() == request.uri().domain()) {
                return get().back();
            }
        }

        return boost::none;
    }

    std::ostream& operator<<(std::ostream& out, const redirects_t& redirects)
    {
        for (const auto& redirect : redirects.get()) {
            out << redirect << "\n";
        }

        return out;
    }


} /* namespace crequests */
