#include "utils.h"
#include "boost_asio.h"

#include <iomanip>
#include <ctime>
#include <iostream>
#include <sstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

namespace crequests {


    bool is_url_encoded(const string_t& value) {
        for (const auto& c : value)
            if (not (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~'))
                return true;
        return false;
    }

    string_t urlencode(const string_t& value) {
        std::ostringstream out;
        out.fill('0');
        out << std::hex;

        for (const auto& c : value) {
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                out << c;
                continue;
            }

            out << std::uppercase;
            out << '%' << std::setw(2) << static_cast<short>(static_cast<unsigned char>(c));
            out << std::nouppercase;
        }

        return out.str();
    }

    string_t urldecode(const string_t& value) {
        std::ostringstream out;
        short c = 0;

        for (auto it = value.cbegin(); it != value.cend(); ++it) {
            if (*it == '%' and it + 1 != value.cend() and it + 2 != value.cend()) {
                std::istringstream in(string_t(it + 1, it + 3));
                in >> std::hex >> c;
                out << static_cast<unsigned char>(c);
                it += 2;
            }
            else if (*it == '+') {
                out << ' ';
            }
            else {
                out << *it;
            }
        }
    
        return out.str();
    }

    string_t trim(const string_t& value) {
        string_t rv(value);
        rv.erase(rv.begin(),
                 std::find_if(rv.begin(), rv.end(),
                              [](char& c)->bool { return !isspace(c); }));
        rv.erase(std::find_if(rv.rbegin(), rv.rend(),
                              [](char& c)->bool { return !isspace(c); }).base(),
                 rv.end());
        return rv;
    }

    string_t tolower(const string_t& value) {
        string_t rv(value);
        std::transform(rv.begin(), rv.end(), rv.begin(), ::tolower);
        return rv;
    }

    string_t toupper(const string_t& value) {
        string_t rv(value);
        std::transform(rv.begin(), rv.end(), rv.begin(), ::toupper);
        return rv;
    }

    string_t compress(const string_t& value) {
        using namespace boost::iostreams;

        std::stringstream compressed;
        std::stringstream origin(value);

        try {
            filtering_streambuf<input> out;
            out.push(gzip_compressor(gzip_params(gzip::best_compression)));
            out.push(origin);
            copy(out, compressed);
        } catch (const std::exception& e) {
            return "";
        }

        return compressed.str();
    }
    
    string_t decompress(const string_t& value) {
        using namespace boost::iostreams;

        std::stringstream compressed(value);
        std::stringstream origin;

        try {
            filtering_streambuf<input> in;
            in.push(gzip_decompressor());
            in.push(compressed);
            copy(in, origin);
        } catch (const std::exception& e) {
            return value;
        }

        return origin.str();
    }

    string_t b64encode(const string_t& value) {
        using namespace boost::archive::iterators;
        using It =
            insert_linebreaks<
                base64_from_binary<
                    transform_width<
                        string_t::const_iterator, 6, 8
                    >
                >,
            76>;
        auto tmp = string_t(It(std::begin(value)), It(std::end(value)));
        
        return tmp.append((3 - value.size() % 3) % 3, '=');
    }

    string_t b64decode(const string_t& value) {
        using namespace boost::archive::iterators;
        using It =
            transform_width<
                binary_from_base64<
                    string_t::const_iterator
                >,
            8, 6>;
        
        return
            boost::algorithm::trim_right_copy_if(
                string_t(It(std::begin(value)), It(std::end(value))),
                [](char c) { return c == '\0'; });
    }

    vector_t<string_t> split(const string_t& value, const char delimiter) {
        vector_t<string_t> rv;
        string_t token;
        std::istringstream stream(value);
        while(std::getline(stream, token, delimiter))
            rv.push_back(token);
        return rv;
    }

    bool is_ip_address(const string_t& domain) {
        ec_t ec;
        boost::asio::ip::address::from_string(domain, ec);
        return not ec;
    }

    string_t time_to_string(const std::time_t& time, const string_t& format) {
        std::tm* tm = std::gmtime(&time);
        if (not tm)
            return "";
        
        std::ostringstream out;
        out << std::put_time(tm, format.c_str());
        return out.str();
    }

    std::time_t string_to_time(const string_t& str, const string_t& format) {
        std::tm tm {};
        
        std::stringstream ss(str);
        ss >> std::get_time(&tm, format.c_str());

        return ss.fail() ? max_time() : timegm(&tm);
    }

    std::time_t now_gmt() {
        std::time_t now = std::time(0);
        std::tm* now_tm = std::gmtime(&now);
        return now_tm ? timegm(now_tm) : 0;
    }

    std::time_t max_time() {
        const auto time_point =
            std::chrono::system_clock::now() + std::chrono::hours(24*365);
        return std::chrono::system_clock::to_time_t(time_point);
    }


} /* namespace crequests */
