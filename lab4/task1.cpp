#include <iostream>
#include <tuple>
#include <type_traits>
#include <fstream>
#include <string>
#include <utility>
#include <iostream>
#include <tuple>
#include <type_traits>

template<typename T>
struct is_tuple : std::false_type {};

template<typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};

template<typename T>
void printElement(std::ostream& os, const T& elem, bool isFirst) {
    if (!isFirst) {
        os << ", ";
    }
    os << elem;
}

template<size_t N>
struct TuplePrinter {
    template<typename... Args>
    static void print(std::ostream& os, const std::tuple<Args...>& t, bool isFirst) {
        TuplePrinter<N - 1>::print(os, t, isFirst);
        printElement(os, std::get<N - 1>(t), false);
    }
};

template<>
struct TuplePrinter<1> {
    template<typename... Args>
    static void print(std::ostream& os, const std::tuple<Args...>& t, bool isFirst) {
        printElement(os, std::get<0>(t), true);
    }
};

template<>
struct TuplePrinter<0> {
    template<typename... Args>
    static void print(std::ostream& os, const std::tuple<Args...>& t, bool isFirst) {
    }
};

template<typename... Args>
void printTuple(std::ostream& os, const std::tuple<Args...>& t) {
    os << "(";
    TuplePrinter<sizeof...(Args)>::print(os, t, true);
    os << ")";
}

template<typename... Args>
std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& t) {
    printTuple(os, t);
    return os;
}
int main() {
    setlocale(LC_ALL,"Russian");
    std::cout << "TEST\n";
    
    std::tuple<int, std::string, double> t1 = std::make_tuple(5, "abcd", 3.14);
    std::cout << "t1 = " << t1 << std::endl;
    
    std::tuple<> t2;
    std::cout << "t2 = " << t2 << std::endl;
    
    std::tuple<std::string> t3 = std::make_tuple("hello");
    std::cout << "t3 = " << t3 << std::endl;
    
    
    
    return 0;
}