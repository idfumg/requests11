#include "headers.h"
#include "utils.h"

#include <sstream>
#include <ostream>

namespace requests {


    headers_t headers_t::from_string(const string_t& str) {
        std::istringstream stream(str);
        headers_t headers;

        string_t header;
        while (std::getline(stream, header) && header != "\r") {
            const auto ind = header.find(":");
            if (ind != string_t::npos)
                headers.emplace(
                    trim(header.substr(0, ind)),
                    trim(header.substr(ind + 1))
                );
        }

        return headers;
    }

    string_t headers_t::to_string() const {
        std::ostringstream out;

        for (auto&& header : *this)
            out << header.first << ": " << header.second << "\r\n";
        out << "\r\n";
            
        return out.str();
    }
    
    void headers_t::update(const headers_t& headers) {
        for (auto&& header : headers)
            this->emplace(header.first, header.second);
    }

    bool headers_t::contains(const string_t& name, const string_t& value) const {
        auto its = this->equal_range(name);
        for (auto it = its.first; it != its.second; ++it)
            if (it->second == value)
                return true;
        return false;
    }

    void headers_t::insert(const string_t& name, const string_t& value) {
        if (tolower(name) != "set-cookie" and this->count(name)) {
            auto its = this->equal_range(name);
            auto it = its.first;
            it->second = value;
        }
        else {
            this->emplace(name, value);
        }
    }

    string_t headers_t::at(const string_t& name) const {
        if (not this->count(name))
            return "";
        
        auto its = this->equal_range(name);
        return its.first->second;
    }

    std::ostream& operator<<(std::ostream& out, const headers_t& headers) {
        for (auto&& header : headers)
            out << header.first << ": " << header.second << "\n";

        return out;
    }

    headers_t operator "" _headers(const char* val, size_t) {
        return headers_t::from_string(val);
    }

    
} /* namespace requests */
