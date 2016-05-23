#ifndef BOOST_ASIO_TYPES_H
#define BOOST_ASIO_TYPES_H

#include <memory>

namespace boost {
    namespace system {
        class error_code;
    }
    namespace asio {
        class io_service;

        namespace ip {
            class tcp;

            template <class T>
            class basic_resolver_iterator;
        }
    }
}

namespace crequests {

    template <class... Args>
    using shared_ptr_t = std::shared_ptr<Args...>;
    
    using ec_t = boost::system::error_code;
    using ioservice_t = boost::asio::io_service;
    using ioservice_ptr_t = shared_ptr_t<ioservice_t>;
    using resolver_iterator_t =
        boost::asio::ip::basic_resolver_iterator<boost::asio::ip::tcp>;

    
} /* namespace crequests */

#endif /* BOOST_ASIO_TYPES_H */
