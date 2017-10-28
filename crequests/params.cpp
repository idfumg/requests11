#include "params.h"
#include "utils.h"

#include <sstream>
#include <iostream>

namespace crequests {


    params_t params_t::from_string(const string_t& str) {
        std::istringstream stream(str);
        params_t params;

        string_t name_value;
        while (std::getline(stream, name_value, '&')) {
            const auto ind = name_value.find("=");
            if (ind != string_t::npos)
                params.insert({
                    name_value.substr(0, ind),
                    name_value.substr(ind + 1)
                });
        }

        return params;
    }

    string_t params_t::to_string() const {
        std::ostringstream out;

        auto it = this->begin();
        while (it != this->end()) {
            out << urlencode(it->first);
            if (not it->second.empty())
                out << "=" << urlencode(it->second);
            ++it;
            if (it != this->end())
                out << "&";
        }
            
        return out.str();
    }

    void params_t::update(const params_t& params) {
        for (const auto& param : params)
            this->operator[](param.first) = param.second;
    }

    std::ostream& operator<<(std::ostream& out, const params_t& params) {
        for (const auto& param : params)
            out << param.first << ": " << param.second << "\n";

        return out;
    }

    params_t operator "" _params(const char* val, size_t) {
        return params_t::from_string(val);
    }

    
} /* namespace crequests */
