#ifndef CONNECTION_H
#define CONNECTION_H

#include "types.h"
#include "error.h"
#include "boost_asio_fwd.h"

namespace crequests {

    class service_t;

    class connection_t {
    public:
        connection_t(service_t& service,
                     const request_t& request);
        connection_t(service_t& service,
                     const request_t& request,
                     const connection_t& connection);
        ~connection_t();
        connection_t(const connection_t& connection);
        connection_t(connection_t&& connection);
        connection_t& operator=(const connection_t& connection);

    public:
        future_t<response_ptr_t> get() const;
        void start();
        bool is_expired() const;

    private:
        friend class conn_impl_t;
        shared_ptr_t<class conn_impl_t> pimpl;
    };

    using connection_ptr_t = shared_ptr_t<connection_t>;

} /* namespace crequests */

#endif /* CONNECTION_H */
