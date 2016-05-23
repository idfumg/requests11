#ifndef SERVICE_H
#define SERVICE_H

#include "macros.h"
#include "types.h"
#include "session.h"
#include "boost_asio_fwd.h"

namespace requests {

    declare_number(dispose_timeout, size_t);

    class service_t : std::enable_shared_from_this<service_t> {
    public:
        service_t();
        service_t(const dispose_timeout_t& dispose_timeout);
        ~service_t();

    public:
        ioservice_t& get_service();
        void run();

        template <class... Args>
        session_ptr_t new_session(Args&&... args) {
            auto session = new_session();
            set_option(session, std::forward<Args>(args)...);
            return session;
        }
        
        session_ptr_t new_session();
    
    private:
        class service_data_t;
        shared_ptr_t<service_data_t> data;
    };

} /* namespace requests */

#endif /* SERVICE_H */
