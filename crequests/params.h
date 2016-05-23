#ifndef PARAMS_H
#define PARAMS_H

#include "types.h"

namespace crequests {

    class params_t : public unordered_map {
        using unordered_map::unordered_map;
        
    public:
        static params_t from_string(const string_t& str);
        string_t to_string() const;
        void update(const params_t& params);
    };

    std::ostream& operator<<(std::ostream& out, const params_t& params);
    params_t operator "" _params(const char* val, size_t);

} /* namespace crequests */

#endif /* PARAMS_H */
