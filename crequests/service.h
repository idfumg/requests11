#ifndef SERVICE_H
#define SERVICE_H

#include "macros.h"
#include "types.h"
#include "session.h"
#include "boost_asio_fwd.h"

namespace crequests {

    declare_number(dispose_timeout, size_t)

    class service_t {
    public:
        service_t();
        service_t(const dispose_timeout_t& dispose_timeout);
        service_t(const service_t& service);
        service_t(service_t&& service);
        service_t& operator=(const service_t& service);
        service_t& operator=(service_t&& service);
        ~service_t();

    public:
        ioservice_t& get_service();
        void run();

        template <class... Args>
        session_t& new_session(Args&&... args) {
            auto& session = new_session();
            set_option(session, std::forward<Args>(args)...);
            return session;
        }
        
        session_t& new_session();
    
    private:
        class service_data_t;
        shared_ptr_t<class service_data_t> data;
    };

} /* namespace crequests */

#endif /* SERVICE_H */
