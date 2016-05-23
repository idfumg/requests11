#include "types.h"

#include <boost/functional/hash.hpp>
#include <boost/algorithm/string/predicate.hpp>

namespace crequests {

    std::size_t iequals::operator()(const string_t& key1,
                                    const string_t& key2) const {
        return boost::algorithm::iequals(key1, key2);
    }

    std::size_t iequals::operator()(const std::pair<string_t, string_t>& pair1,
                                    const std::pair<string_t, string_t>& pair2) const {
        return
            boost::algorithm::iequals(pair1.first, pair2.first) and
            boost::algorithm::iequals(pair1.second, pair2.second);
    }

    std::size_t ihash::operator()(const string_t& key) const {
        std::size_t seed = 0;
        for (auto& c : key)
            boost::hash_combine(seed, tolower(c));
        return seed;
    }

    std::size_t ihash::operator()(const std::pair<string_t, string_t>& pair) const {
        std::size_t seed = 0;
        for (auto& c : pair.first)
            boost::hash_combine(seed, tolower(c));
        for (auto& c : pair.second)
            boost::hash_combine(seed, tolower(c));
        return seed;
    }

} /* namespace crequests */
