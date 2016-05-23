#ifndef MACROS_H
#define MACROS_H

#include <iostream>

#define declare_string(class_name) \
    class class_name##_t { \
    public: \
        explicit class_name##_t() = default; \
        explicit class_name##_t(const string_t& arg) : val(arg) {}   \
        class_name##_t(const class_name##_t& arg) = default; \
        class_name##_t(class_name##_t&& arg) = default;                 \
        class_name##_t& operator = (const class_name##_t& arg) = default; \
        class_name##_t& operator = (class_name##_t&& arg) = default;    \
        \
        bool operator==(const class_name##_t& rhs) const { \
            return val == rhs.val; \
        } \
        bool operator!=(const class_name##_t& rhs) const { \
            return val != rhs.val; \
        } \
        \
        const string_t& value() const { return val; } \
        string_t& value() { return val; } \
        bool empty() const { return val.empty(); } \
        \
    private: \
        string_t val {}; \
    }; \
    inline std::istream& operator>>(std::istream& in, class_name##_t& arg) { \
        in >> arg.value();                                          \
        return in; \
    } \
    inline std::ostream& operator<<(std::ostream& out, const class_name##_t& arg) {\
        out << arg.value();                                          \
        return out; \
    } \
    inline class_name##_t operator "" _##class_name(const char* val, size_t) {\
        return class_name##_t { val }; \
    }

#define declare_number(class_name, int_t) \
    class class_name##_t { \
    public: \
        explicit class_name##_t() = default; \
        explicit class_name##_t(int_t arg) : val(arg) {}   \
        class_name##_t(const class_name##_t& arg) = default;            \
        class_name##_t(class_name##_t&& arg) = default;                 \
        class_name##_t& operator = (const class_name##_t& arg) = default; \
        class_name##_t& operator = (class_name##_t&& arg) = default;    \
        \
        bool operator==(const class_name##_t& rhs) const { \
            return val == rhs.val; \
        } \
        bool operator!=(const class_name##_t& rhs) const { \
            return val != rhs.val; \
        } \
        const int_t& value() const { return val; } \
        int_t& value() { return val; } \
        \
    private: \
        int_t val {}; \
    }; \
    inline std::istream& operator>>(std::istream& in, class_name##_t& arg) { \
        in >> arg.value();                                          \
        return in; \
    } \
    inline std::ostream& operator<<(std::ostream& out, const class_name##_t& arg) {\
        out << arg.value();                                          \
        return out; \
    } \
    template <char... Chars> \
    inline class_name##_t operator "" _##class_name() { \
        return class_name##_t { Chars... }; \
    }
    
#define declare_bool(class_name) \
    class class_name##_t {                                              \
    public:                                                             \
        explicit class_name##_t() = default;                            \
        explicit class_name##_t(const bool arg) : val(arg) {}           \
        class_name##_t(const class_name##_t& arg) = default;            \
        class_name##_t(class_name##_t&& arg) = default;                 \
        class_name##_t& operator = (const class_name##_t& arg) = default; \
        class_name##_t& operator = (class_name##_t&& arg) = default;    \
                                                                        \
        bool operator==(const class_name##_t& rhs) const { \
            return val == rhs.val; \
        } \
        bool operator!=(const class_name##_t& rhs) const { \
            return val != rhs.val; \
        } \
        operator bool() const { \
            return val; \
        }                                  \
        const bool& value() const { return val; } \
        bool& value() {return val; } \
        \
    private: \
        bool val { false }; \
    }; \
    inline std::istream& operator>>(std::istream& in, class_name##_t& arg) { \
        in >> arg.value();                                          \
        return in; \
    } \
    inline std::ostream& operator<<(std::ostream& out, const class_name##_t& arg) {\
        out << arg.value();                                          \
        return out; \
    } \
    template <char... Chars> \
    inline class_name##_t operator "" _##class_name() {\
        return class_name##_t { Chars... }; \
    }

#endif /* MACROS_H */
