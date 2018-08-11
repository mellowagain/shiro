#ifndef SHIRO_COMMON_TABLES_HH
#define SHIRO_COMMON_TABLES_HH

#define object_detailed_struct(name, var, type)                             \
struct var {                                                               \
struct _alias_t {                                                           \
    static constexpr const char _literal[] = name;                         \
    using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;  \
    template <typename T>                                                   \
    struct _member_t {                                                      \
        T var;                                                             \
        T &operator()() {                                                   \
            return var;                                                    \
        }                                                                   \
        const T &operator()() const {                                       \
            return var;                                                    \
        }                                                                   \
    };                                                                      \
};                                                                          \
using _traits = sqlpp::make_traits<type>;                                   \
}                                                                           \

#define object_struct(name, type) object_detailed_struct(#name, name, type)

#endif  // SHIRO_COMMON_TABLES_HH
