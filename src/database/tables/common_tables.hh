#ifndef SHIRO_COMMON_TABLES_HH
#define SHIRO_COMMON_TABLES_HH

#define object_struct(name, type)                                           \
struct name {                                                               \
struct _alias_t {                                                           \
    static constexpr const char _literal[] = #name;                         \
    using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;  \
    template <typename T>                                                   \
    struct _member_t {                                                      \
        T name;                                                             \
        T &operator()() {                                                   \
            return name;                                                    \
        }                                                                   \
        const T &operator()() const {                                       \
            return name;                                                    \
        }                                                                   \
    };                                                                      \
};                                                                          \
using _traits = sqlpp::make_traits<type>;                                   \
}                                                                           \

#endif  // SHIRO_COMMON_TABLES_HH
