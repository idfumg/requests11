#ifndef API_H
#define API_H

#include "response.h"
#include "service.h"
#include "session.h"

namespace requests {


    template <class SessionT, class Head>
    void set_option(SessionT&& session, Head&& head) {
        session->set_option(std::forward<Head>(head));
    }
    
    template <class SessionT, class Head, class... Tail>
    void set_option(SessionT&& session, Head&& head, Tail&&... tail) {
        set_option(session, std::forward<Head>(head));
        set_option(session, std::forward<Tail>(tail)...);
    }

    template <class ServiceT, class... Args>
    response_ptr_t Get(ServiceT&& service, Args&& ...args) {
        auto session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session->Get();
    }

    template <class ServiceT, class... Args>
    response_ptr_t Post(ServiceT&& service, Args&& ...args) {
        auto session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session->Post();
    }
    
    template <class ServiceT, class... Args>
    response_ptr_t Put(ServiceT&& service, Args&& ...args) {
        auto session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session->Put();
    }

    template <class ServiceT, class... Args>
    response_ptr_t Patch(ServiceT&& service, Args&& ...args) {
        auto session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session->Patch();
    }

    template <class ServiceT, class... Args>
    response_ptr_t Delete(ServiceT&& service, Args&& ...args) {
        auto session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session->Delete();
    }

    template <class ServiceT, class... Args>
    response_ptr_t Head(ServiceT&& service, Args&& ...args) {
        auto session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session->Head();
    }

    template <class ServiceT, class... Args>
    future_t<response_ptr_t> AsyncGet(ServiceT&& service, Args&& ...args) {
        auto session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session->AsyncGet();
    }

    template <class ServiceT, class... Args>
    future_t<response_ptr_t> AsyncPost(ServiceT&& service, Args&& ...args) {
        auto session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session->AsyncPost();
    }
    
    template <class ServiceT, class... Args>
    future_t<response_ptr_t> AsyncPut(ServiceT&& service, Args&& ...args) {
        auto session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session->AsyncPut();
    }

    template <class ServiceT, class... Args>
    future_t<response_ptr_t> AsyncPatch(ServiceT&& service, Args&& ...args) {
        auto session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session->AsyncPatch();
    }

    template <class ServiceT, class... Args>
    future_t<response_ptr_t> AsyncDelete(ServiceT&& service, Args&& ...args) {
        auto session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session->AsyncDelete();
    }

    template <class ServiceT, class... Args>
    future_t<response_ptr_t> AsyncHead(ServiceT&& service, Args&& ...args) {
        auto session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session->AsyncHead();
    }
    
} /* namespace requests */

#endif /* API_H */
