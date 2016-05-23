#ifndef BOOST_ASIO_H
#define BOOST_ASIO_H

#include "boost_asio_fwd.h"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/steady_timer.hpp>

namespace requests {
    
    using tcp_socket_t = boost::asio::ip::tcp::socket;
    using tcp_socket_ptr_t = shared_ptr_t<tcp_socket_t>;
    using ssl_socket_t = boost::asio::ssl::stream<tcp_socket_t>;
    using ssl_socket_ptr_t = shared_ptr_t<ssl_socket_t>;
    using resolver_t = boost::asio::ip::tcp::resolver;
    using timer__t = boost::asio::steady_timer;
    using timer_ptr_t = shared_ptr_t<timer__t>;
    using streambuf_t = boost::asio::streambuf;
    using work_t = boost::asio::io_service::work;
    using work_ptr_t = shared_ptr_t<work_t>;
    using timer__t = boost::asio::steady_timer;
    using timer_ptr_t = shared_ptr_t<timer__t>;
    using strand_t = boost::asio::io_service::strand;

} /* namespace requests */

#endif /* BOOST_ASIO_H */
