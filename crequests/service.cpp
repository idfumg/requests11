#include "service.h"
#include "request.h"
#include "connection.h"
#include "boost_asio.h"

#include <thread>
#include <list>

namespace crequests {


    namespace {

        using thread_t = std::thread;

    } /* anonymous namespace */


    /************************************************************
     * service__data_t section.
     ************************************************************/


    class service_t::service_data_t {
    public:
        service_data_t(const dispose_timeout_t& dispose_timeout);
        service_data_t(dispose_timeout_t&& dispose_timeout);
        ~service_data_t();

    public:
        ioservice_t& get_service();
        void add_session(const session_ptr_t& session);
        void set_dispose_timer();
        void on_dispose_timer(const ec_t& ec);
        void start();
        void run();

    private:
        ioservice_t ioservice {};
        work_ptr_t work { std::make_shared<work_t>(ioservice) };
        strand_t strand { ioservice };
        timer__t dispose_timer { ioservice };
        std::list<session_ptr_t> sessions;
        std::unique_ptr<std::thread> thread;
        dispose_timeout_t dispose_timeout { 1 };
    };

    service_t::service_data_t::service_data_t(const dispose_timeout_t& dispose_timeout_)
        : dispose_timeout(dispose_timeout_)
    {}

    service_t::service_data_t::service_data_t(dispose_timeout_t&& dispose_timeout_)
        : dispose_timeout(std::move(dispose_timeout_))
    {}

    service_t::service_data_t::~service_data_t() {
        work.reset();
        ioservice.stop();

        if (thread and thread->joinable())
            thread->join();
    }

    void service_t::service_data_t::start() {
        thread.reset(new thread_t([this](){
            ioservice.run();
        }));

        set_dispose_timer();
    }

    void service_t::service_data_t::run() {
        ioservice.run();
    }

    ioservice_t& service_t::service_data_t::get_service() {
        return ioservice;
    }

    void service_t::service_data_t::add_session(const session_ptr_t& session) {
        sessions.push_back(session);
    }

    void service_t::service_data_t::set_dispose_timer() {
        dispose_timer.expires_from_now(
            seconds_t{ dispose_timeout.value() });
        auto callback = [this](const ec_t& ec) {
            on_dispose_timer(ec);
        };
        dispose_timer.async_wait(strand.wrap(callback));
    }
    
    void service_t::service_data_t::on_dispose_timer(const ec_t& ec) {
        if (ec)
            return;
        
        auto&& it = sessions.begin();
        while (it != sessions.end()) {
            if ((*it)->is_expired()) {
                auto it_to_erase = it;
                it++;
                sessions.erase(it_to_erase);
            }
            else {
                it++;
            }
        }

        set_dispose_timer();
    }


    /************************************************************
     * service_t section.
     ************************************************************/


    service_t::service_t()
        : service_t(dispose_timeout_t { 1 })
    {
        
    }

    service_t::service_t(const dispose_timeout_t& dispose_timeout)
        : data(std::make_shared<service_data_t>(dispose_timeout))
    {
        data->start();
    }
    
    service_t::~service_t() {
        
    }

    ioservice_t& service_t::get_service() {
        return data->get_service();
    }

    session_ptr_t service_t::new_session() {
        auto session = std::make_shared<session_t>(*this);
        data->add_session(session);
        return session;
    }

    void service_t::run() {
        data->run();
    }


} /* namespace crequests */
