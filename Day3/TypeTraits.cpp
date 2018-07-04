#include <iostream>

template<class T, class S>
T Serialize(S source) {
    static_assert(sizeof(S) == sizeof(T),
        "Target and source types shall be of equal size!");
    static_assert(std::is_integral<T>::value,
        "Target type shall be integral!");
    return *(reinterpret_cast<T*>(&source)); // DON’T DO THIS!
}

template<class T, class S>
T Deserialize(S source) {
    static_assert(sizeof(S) == sizeof(T),
        "Target and source types shall be of equal size!");
    static_assert(std::is_integral<S>::value,
        "Target type shall be integral!");
    return *(reinterpret_cast<T*>(&source)); // DON’T DO THIS!
}

int main()
{
    float f = 4.567f;
    int f_ser = Serialize<int>(f); // OK
    float f_deser = Deserialize<float>(f_ser);
    int i_deser = Deserialize<int>(f_deser);

    
    std::cout << "f = " << f << std::endl
        << "f_ser = " << f_ser << std::endl
        << "f_deser = " << f_deser << std::endl;
}
