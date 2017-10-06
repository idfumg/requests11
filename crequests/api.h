#ifndef API_H
#define API_H

#include "response.h"
#include "asyncresponse.h"
#include "service.h"
#include "session.h"

namespace crequests {


    template <class SessionT, class Head>
    void set_option(SessionT& session, Head&& head) {
        session.set_option(std::forward<Head>(head));
    }
    
    template <class SessionT, class Head, class... Tail>
    void set_option(SessionT& session, Head&& head, Tail&&... tail) {
        set_option(session, std::forward<Head>(head));
        set_option(session, std::forward<Tail>(tail)...);
    }

    template <class ServiceT, class... Args>
    response_t Get(ServiceT&& service, Args&& ...args) {
        auto& session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session.Get();
    }

    template <class ServiceT, class... Args>
    response_t Post(ServiceT&& service, Args&& ...args) {
        auto& session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session.Post();
    }
    
    template <class ServiceT, class... Args>
    response_t Put(ServiceT&& service, Args&& ...args) {
        auto& session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session.Put();
    }

    template <class ServiceT, class... Args>
    response_t Patch(ServiceT&& service, Args&& ...args) {
        auto& session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session.Patch();
    }

    template <class ServiceT, class... Args>
    response_t Delete(ServiceT&& service, Args&& ...args) {
        auto& session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session.Delete();
    }

    template <class ServiceT, class... Args>
    response_t Head(ServiceT&& service, Args&& ...args) {
        auto& session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session.Head();
    }

    template <class ServiceT, class... Args>
    asyncresponse_t AsyncGet(ServiceT&& service, Args&& ...args) {
        auto& session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session.AsyncGet();
    }

    template <class ServiceT, class... Args>
    asyncresponse_t AsyncPost(ServiceT&& service, Args&& ...args) {
        auto& session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session.AsyncPost();
    }
    
    template <class ServiceT, class... Args>
    asyncresponse_t AsyncPut(ServiceT&& service, Args&& ...args) {
        auto& session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session.AsyncPut();
    }

    template <class ServiceT, class... Args>
    asyncresponse_t AsyncPatch(ServiceT&& service, Args&& ...args) {
        auto& session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session.AsyncPatch();
    }

    template <class ServiceT, class... Args>
    asyncresponse_t AsyncDelete(ServiceT&& service, Args&& ...args) {
        auto& session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session.AsyncDelete();
    }

    template <class ServiceT, class... Args>
    asyncresponse_t AsyncHead(ServiceT&& service, Args&& ...args) {
        auto& session = service.new_session();
        set_option(session, std::forward<Args>(args)...);
        return session.AsyncHead();
    }
    
} /* namespace crequests */

#endif /* API_H */
