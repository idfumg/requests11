#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include "../crequests/boost_asio_fwd.h"

namespace requests {

    class server_t {
    public:
        server_t(const std::string& address,
                 const std::string& port,
                 bool is_ssl = false);
        ~server_t();

    public:
        void run();
        void stop();
        void do_accept();

    private:
        ioservice_t io_service;
        boost::asio::ip::tcp::acceptor acceptor;
        bool is_ssl;
    };
    
} /* namespace requests */

#endif /* SERVER_H */
