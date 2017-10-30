#ifndef CONNECTION_H
#define CONNECTION_H

#include "boost_asio_fwd.h"
#include "error.h"
#include "types.h"

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
        connection_t& operator=(connection_t&& connection);

    public:
        /*
          Function which gives us an object for the future response.
          This response can be obtained when the current connection
          is done (good response or an error on any step, does not matter).
        */
        future_t<response_t> get() const;

        /*
          This function starts an asynchronous connection.
          This connection will ends up in a background process.
         */
        void start();

        /*
          This function say us that the current connection is expired.
          This means the current connection ends up + waited dispose
          timeout and removed and no longer live. You must fetch response
          before this timeout expired.
         */
        bool is_expired() const;

    private:
        friend class conn_impl_t;
        shared_ptr_t<class conn_impl_t> pimpl;
    };


} /* namespace crequests */

#endif /* CONNECTION_H */
