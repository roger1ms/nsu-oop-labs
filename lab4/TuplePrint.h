#include <iostream>
#include <tuple>
#include <type_traits>
#include <fstream>
#include <string>
#include <utility>

//1 вариант через integer_sequence
// template <typename Tuple, typename std::size_t... Ints>
// void print(std::ostream& os, Tuple& t, std::integer_sequence<std::size_t, Ints...>) {
//     ((os << (Ints==0 ? "" : ", ") << std::get<Ints>(t)), ...);
// }

// template <typename... Args>
// std::ostream& operator<<(std::ostream& os, const std::tuple<Args...> t) {
//     os << "(";
//     print(os, t, std::make_integer_sequence<std::size_t, sizeof...(Args)>{});
//     os << ")";
//     return os;
// }

//2 вариант с лямбда функцией
// template<typename... Args>
// std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& t) {
//     os << "(";
    
//     [&]<std::size_t... Ints>(std::index_sequence<Ints...>) {
//         ((os << (Ints == 0 ? "" : ", ") << std::get<Ints>(t)), ...);
//     }(std::make_index_sequence<sizeof...(Args)>{});
    
//     os << ")";
//     return os;
// }

//3 вариант с apply
// template<typename... Args>
// std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& t) {
//     os << "(";
    
//     std::apply([&os](const auto&... args) {
//         std::size_t n = 0;
//         ((os << (n++ == 0 ? "" : ", ") << args), ...);
//     }, t);
    
//     os << ")";
//     return os;
// }

//4 вариант if constexpr
template<std::size_t I = 0, typename... Ts>
void print_tuple(std::ostream& os, const std::tuple<Ts...>& t) {
    if constexpr (I < sizeof...(Ts)) {
        if constexpr (I == 0) {
            os << std::get<I>(t);
        } else {
            os << " | " << std::get<I>(t);
        }
        print_tuple<I + 1>(os, t);
    }
}

template<typename... Ts>
std::ostream& operator<<(std::ostream& os, const std::tuple<Ts...>& t) {
    print_tuple(os, t);
    return os;
}
