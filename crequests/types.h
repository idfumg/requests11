#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <chrono>
#include <memory>
#include <future>
#include <unordered_map>
#include <vector>

#include <boost/optional.hpp>

namespace requests {

    using string_t = std::string;
    template <class T>
    using vector_t = std::vector<T>;
    template <class T> using optional_t = boost::optional<T>;
    using seconds_t = std::chrono::seconds;
    template <class... Args>
    using shared_ptr_t = std::shared_ptr<Args...>;
    template <class T>
    using future_t = std::shared_future<T>;
    template <class T>
    using promise_t = std::promise<T>;
    
    class iequals {
    public:
        std::size_t operator()(const string_t& key1,
                               const string_t& key2) const;
        std::size_t operator()(const std::pair<string_t, string_t>& pair1,
                               const std::pair<string_t, string_t>& pair2) const;
    };

    class ihash {
    public:
        std::size_t operator()(const string_t& key) const;
        std::size_t operator()(const std::pair<string_t, string_t>& pair) const;
    };

    using unordered_map = std::unordered_map<string_t, string_t, ihash, iequals>;

    class response_t;
    using response_ptr_t = shared_ptr_t<response_t>;
    class request_t;
    class service_t;
    
    using final_callback_t = std::function<void(const response_t& response)>;
}

#endif /* TYPES_H */
